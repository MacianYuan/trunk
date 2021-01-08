#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()
#include <string.h>
#include <memory>

#include <iostream>
#include <memory>
#include <sstream>
#include <map>
#include <fstream>

#include <curl/curl.h>
#include <openssl/md5.h>
#include <QtCore>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>

#include "ftp/QTFtpInfo.h"
#include "ftp/QTFtpTools.h"
#include "tools/log.h"

static size_t ReadCallBack(void* ptr, size_t size, size_t nmemb, void* stream);
static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

struct FtpTransfer {
    QFile* fp;
    MD5_CTX* md5Ctx;
};

struct InnerProgress {
    time_t beginTime;
    time_t endTime;

    QString dirName;
    qint64 dirSize;
    qint64 dirSendSize;

    QString fileName;
    qint64 fileSize;
    qint64 fileBeforeSendSize;
    qint64 fileNowSendSize;
    QString createTime;

    quint64 id;
    unsigned int userId;
    unsigned int instrumentId;
    unsigned int group_id;
    unsigned int actived_account;

    InnerProgress() {
        DirClear();
    }

    void FileClear() {
        fileName.clear();
        fileSize = 0;
        fileBeforeSendSize = 0;
        fileNowSendSize = 0;
    }

    void DirClear() {
        beginTime = 0;
        endTime = 0;

        dirName.clear();
        dirSize = 0;
        dirSendSize = 0;

        FileClear();

        id = 0;
        userId = 0;
        instrumentId = 0;
        group_id = 0;
        actived_account = 0;
    }
};

QTFtpInfo::QTFtpInfo(UpdateUploadProgress cb, void* ctx, QObject* parent /*= NULL*/) : QObject(parent)
{
    m_ProgressCB = cb;
    m_Ctx = ctx;
    m_Progress = new InnerProgress;
    m_NeedStop = false;
    smbIsRun = false;

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(stopThread()));
}

QTFtpInfo::~QTFtpInfo()
{
    delete m_Progress;
}

bool QTFtpInfo::InitDir(QString sambaPath)
{
    m_SambaRoot.clear();
    if ('/' == sambaPath.at(sambaPath.length() - 1)) {
        sambaPath.remove(sambaPath.length() - 1, 1);
    }
    m_SambaRoot = sambaPath;

    QTFtpTools::CreateDir(m_SambaRoot);

    /// 获取当前samba根目录下，有多少文件夹需要发送
    QDir dir(m_SambaRoot);
    std::map<time_t, QString> tempDirList;
    std::lock_guard<std::mutex> guard(m_Mutex);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i != folder_list.size(); i++) {
        QFileInfo fileInfo = folder_list.at(i);
        QString name = fileInfo.fileName();
        //only exp
        QRegExp exp("[0-9]+_[0-9]+_[0-9]+");
        if (name.indexOf(exp) < 0)
        {
            continue;
        }
//        if (name == "sambaRootPath") {
//            continue;
//        }
        QDateTime md = fileInfo.lastModified();
        time_t mdt = md.toTime_t();
        tempDirList.insert(std::make_pair(mdt, name));
    }

    for (auto v : tempDirList) {
        m_NeedSendDir.push_back(v.second);
    }

    return true;
}

bool QTFtpInfo::InitFtp(FtpLogin& l)
{
    login.name = l.name;
    login.passwd = l.passwd;
    login.host = l.host;
    login.port = l.port;
    login.rootpath = l.rootpath;
    if (l.rootpath.length() > 1 && '/' == l.rootpath.at(l.rootpath.length() - 1)) {
        login.rootpath.remove(l.rootpath.length() - 1, 1);
    }
    login.maxSpeed = l.maxSpeed;

    DEBUG_LOG("ftp user name is :" << login.name.toLocal8Bit().data());
    DEBUG_LOG("ftp user passwd is :" << login.passwd.toLocal8Bit().data());
    DEBUG_LOG("ftp host ip is :" << login.host.toLocal8Bit().data());
    DEBUG_LOG("ftp ftp port is :" << login.port);
    DEBUG_LOG("ftp root path is :" << login.rootpath.toLocal8Bit().data());
    DEBUG_LOG("ftp max speed is :" << login.maxSpeed);


    return true;
}

bool QTFtpInfo::Run()
{
    while (false == m_NeedStop) {
        sleep(1);
        QString dir;
        {
            std::lock_guard<std::mutex> guard(m_Mutex);
            if (false == m_NeedSendDir.empty()) {
                dir = m_NeedSendDir.front();
                m_NeedSendDir.pop_front();
            }
        }

        if (0 == dir.length()) {
            continue;
        }

        BeforeUpload(dir);
        if (false == Upload(dir, true)) {
            std::lock_guard<std::mutex> guard(m_Mutex);
            m_NeedSendDir.push_front(dir);
        } else {
            AfterUpload(dir);
        }

        //while (false == Upload(dir, true)) {
        //    sleep(1); /// 上传失败，重试
        //}
    }

    DEBUG_LOG("upgarde:exit QTFtpInfo thread");

    return true;
}

bool QTFtpInfo::HandlerException(quint64 id, FtpSessionInfo sessionInfo, bool netStatus)
{
    std::stringstream ss;
    ss << sessionInfo.user_id << "_" << sessionInfo.instrument_id << "_" << id;// << "_" << time(NULL);
    QString sambaPath(ss.str().c_str());

    QDir tempDir;
    DEBUG_LOG(", now samba path is [" << sambaPath.toLocal8Bit().data() << "]");
    if (false == tempDir.exists(m_SambaRoot + "/ftp/sambaRootPath")) {  /// 之前用户没有ftp信息
        DEBUG_LOG(", start new ftp session");
        QString temp;
        UserLogin(id, sessionInfo, netStatus, temp);
    } else {   ///生成的ftp信息，看网络状态，是否启动samba
        DEBUG_LOG(", parse before ftp session");
        tempDir.rename(m_SambaRoot + "/ftp/sambaRootPath", m_SambaRoot + "/" + sambaPath);
        m_NowSambaDir = sambaPath;
        if (true == netStatus) { /// 之前的网络状态没有记录，依靠现有的网络状态
            StartSmb(m_NowSambaDir);
        }
    }

    return true;
}

bool QTFtpInfo::UserLogin(quint64 id, FtpSessionInfo sessionInfo, bool netStatus, QString& sambaPath)
{
    bool status = false;

    do {
        m_NowSambaDir.clear();

        std::stringstream ss;
        ss << sessionInfo.user_id << "_" << sessionInfo.instrument_id << "_" << id;
        sambaPath = QString(ss.str().c_str());
        QString localPath = m_SambaRoot + "/" + sambaPath;
        DEBUG_LOG("samba path is [" << sambaPath.toLocal8Bit().data() << "]");

        std::lock_guard<std::mutex> guard(m_Mutex);

        std::ofstream out;
        out.open(std::string(m_SambaRoot.toLatin1().data()) + "/ftp_" + sambaPath.toLatin1().data() + ".conf");
        if (false == out.good()) {
            break;
        }
        out << "id=" << id << "\n";
        out << "user_id=" << sessionInfo.user_id << "\n";
        out << "insturment_id=" << sessionInfo.instrument_id << "\n";
        out << "group_id=" << sessionInfo.group_id << "\n";;
        out << "actived_account=" << sessionInfo.actived_account << "\n";
        out.close();

        m_NowSambaDir = sambaPath;
        DEBUG_LOG("m_NowSambaDir is [" << m_NowSambaDir.toLocal8Bit().data() << "]");
        DEBUG_LOG("samba path is [" << sambaPath.toLocal8Bit().data() << "]");
        if (true == netStatus) {
            StartSmb(m_NowSambaDir);
        }

        status = true;
    } while (false);

    return status;
}

bool QTFtpInfo::UserLogout()
{
    QDir tempDir;
    if (true == tempDir.exists(m_SambaRoot + "/ftp/sambaRootPath") && 0 < m_NowSambaDir.length()) {
        tempDir.rename(m_SambaRoot + "/ftp/sambaRootPath", m_SambaRoot + "/" + m_NowSambaDir);
        DEBUG_LOG("File size is : " << QTFtpTools::GetFileList(m_SambaRoot + "/" + m_NowSambaDir).size());
        if (QTFtpTools::GetFileList(m_SambaRoot + "/" + m_NowSambaDir).size() > 0) {
            std::lock_guard<std::mutex> guard(m_Mutex);
            m_NeedSendDir.push_back(m_NowSambaDir);
        } else {
            QTFtpTools::DeletePath(m_SambaRoot + "/" + m_NowSambaDir);
            QTFtpTools::DeletePath(m_SambaRoot + "/ftp_" + m_NowSambaDir + ".conf");
        }
    }
    m_NowSambaDir.clear();
    smbIsRun = false;

    return true;
}

bool QTFtpInfo::ManualStartSmb()
{
    if (m_NowSambaDir.length() > 0) {
        StartSmb(m_NowSambaDir);
    }
    return true;
}

bool QTFtpInfo::ManualStopSmb()
{
    system("killall vsftpd");
    return true;
}

void QTFtpInfo::UpdateProgress(long ul)
{
    //DEBUG_LOG("call this function[" << __FUNCTION__ << "]");
    m_Progress->fileNowSendSize = ul;
    time_t here = time(NULL) - m_Progress->beginTime;
    /// 每隔10秒发送一次
    if (NULL != m_ProgressCB && 0 == here % 10) {
        Progress progress;
        GetProgress(progress);
        DEBUG_LOG("send percentage:" << progress.percentage);
        m_ProgressCB(m_Ctx, progress);
    }
}

bool QTFtpInfo::GetProgress(Progress& info)
{
    ResetProgress(info);

    if (0 == m_Progress->dirName.length()) {
        return false;
    }
    m_Progress->endTime = time(NULL);

    info.id = m_Progress->id;
    info.userId = m_Progress->userId;
    info.instrumentId = m_Progress->instrumentId;
    info.group_id = m_Progress->group_id;
    info.actived_account = m_Progress->actived_account;

    info.fileName = m_Progress->fileName;
    info.fileSize = m_Progress->fileSize;
    info.createTime = m_Progress->createTime;
    info.path = login.rootpath + "/" + m_Progress->dirName;
    info.consume_time = m_Progress->endTime - m_Progress->beginTime;
    long long templl = m_Progress->fileNowSendSize;
    templl += m_Progress->fileBeforeSendSize;
    info.percentage = (m_Progress->fileSize == 0 ? 100 : (int)(templl * 100 / m_Progress->fileSize));

    return true;
}

/**
 * @brief 退出线程
 */
void QTFtpInfo::stopThread()
{
    m_NeedStop = true;
    DEBUG_LOG("upgarde:exit QTFtpInfo thread");
    //延时2秒，等线程退出
    sleep(2);
}

bool QTFtpInfo::Upload(QString path, bool needDelete /*= true*/)
{
    bool status = false;

    /// 发送多个文件
    QFileInfoList dirList = QTFtpTools::GetFileList(m_SambaRoot + "/" + path);
    auto iter = dirList.begin();
    while (dirList.end() != iter) {
        /// 获取ftp服务器上，文件已经发送的大小
        QString localPath = iter->absoluteFilePath();
        DEBUG_LOG("local path is :" << localPath.toLocal8Bit().data());
        QString sonPath = localPath.remove(0, m_SambaRoot.length() + 1);
        QString ftpPath = login.rootpath + "/" + sonPath;

        qint64 startPos = 0;
        if (false == FtpServerFileSize(ftpPath, startPos)) {
            sleep(1);
            continue;
        }

        /// 初始化文件发送的进度信息
        InitSendFile(sonPath, startPos);

        /// 发送文件
        status = SendFile(sonPath, startPos);
        if (false == status) {
            std::cout << "send file failed" << std::endl;
            continue;
        }

        /// 发送文件之后，进度信息调整
        EndSendFile(status);

        if (true == needDelete) {
            QTFtpTools::DeletePath(iter->absoluteFilePath());
        }

        ++iter;
    }

    if (true == needDelete) {
        QTFtpTools::DeletePath(m_SambaRoot + "/" + path);
    }

    return true;
}

bool QTFtpInfo::BeforeUpload(QString path)
{
    bool status = false;

    do {
        QString localPath = m_SambaRoot + "/" + path;
        m_Progress->DirClear();
        //m_Progress->dirName = path;
        m_Progress->dirSize = QTFtpTools::GetDirSize(localPath);

        std::ifstream in;
        in.open(m_SambaRoot.toStdString() + "/ftp_" + path.toStdString() + ".conf");
        if (false == in.good()) {
            break;
        }
        char line[1024] = { 0 };

        memset(line, 0x00, sizeof(line));
        in.getline(line, sizeof(line) - 1);
        sscanf(line, "%*[^=]=%llu", &m_Progress->id);

        memset(line, 0x00, sizeof(line));
        in.getline(line, sizeof(line) - 1);
        sscanf(line, "%*[^=]=%u", &m_Progress->userId);

        memset(line, 0x00, sizeof(line));
        in.getline(line, sizeof(line) - 1);
        sscanf(line, "%*[^=]=%u", &m_Progress->instrumentId);

        memset(line, 0x00, sizeof(line));
        in.getline(line, sizeof(line) - 1);
        sscanf(line, "%*[^=]=%u", &m_Progress->group_id);

        memset(line, 0x00, sizeof(line));
        in.getline(line, sizeof(line) - 1);
        sscanf(line, "%*[^=]=%u", &m_Progress->actived_account);

        in.close();

        status = true;
    } while (false);

    return status;
}

class UseTime
{
public:
    UseTime(QString n) {
        name = n;
        begin = time(NULL);
    }
    ~UseTime() {
        end = time(NULL);
        Logger logRoot = Logger::getRoot();
        DEBUG_LOG("send file [" << name.toLocal8Bit().data() << "] use time " << (end - begin));
    }
private:
    time_t begin;
    time_t end;
    QString name;
};

bool QTFtpInfo::SendFile(QString path, qint64 startPos)
{
    UseTime ut(path);

    bool status = false;
    QString localPath = m_SambaRoot + "/" + path;
    QString ftpPath = login.rootpath + "/" + path;

    DEBUG_LOG("send file " << path.toLocal8Bit().data());

    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit errorOccured(tr("SD card error，please reformat or replace"));//SD卡错误，请重新格式化或更换
        DEBUG_LOG("open file failed:" << path.toLocal8Bit().data());
        return false;
    }


    std::shared_ptr<CURL> handler(curl_easy_init(), curl_easy_cleanup);
    FtpTransfer transfer;
    transfer.fp = &file;
    transfer.md5Ctx = &m_Md5Ctx;

    DEBUG_LOG("send file " << path.toLocal8Bit().data());
    do {
        if (!handler) {
            break;
        }

        DEBUG_LOG("send file " << path.toLocal8Bit().data());
        curl_easy_setopt(handler.get(), CURLOPT_READFUNCTION, ReadCallBack); /// 读取数据
        curl_easy_setopt(handler.get(), CURLOPT_READDATA, &transfer); /// 读取数据的指针
        curl_easy_setopt(handler.get(), CURLOPT_PROGRESSFUNCTION, ProgressCallback);
        curl_easy_setopt(handler.get(), CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(handler.get(), CURLOPT_PROGRESSDATA, this);
        curl_easy_setopt(handler.get(), CURLOPT_UPLOAD, 1L); /// 使用ftp的upload
        curl_easy_setopt(handler.get(), CURLOPT_USERNAME, login.name.toLocal8Bit().data());
        curl_easy_setopt(handler.get(), CURLOPT_PASSWORD, login.passwd.toLocal8Bit().data());
        //QString url = QString("ftp://") + login.name + ":" + login.passwd + "@" + login.host + ":" + login.port + ftpPath;
        QString url = QString("ftp://") + login.host + ":" + QString::number(login.port) + ftpPath;
        QTFtpTools::convertSpecialChar(url);
        DEBUG_LOG("send file ftp path is " << url.toLocal8Bit().data());
        curl_easy_setopt(handler.get(), CURLOPT_URL, url.toLocal8Bit().data()); /// 设置url
        curl_easy_setopt(handler.get(), CURLOPT_FTP_CREATE_MISSING_DIRS, 1L); /// 缺失目录，自动创建

        DEBUG_LOG("max speed : " << login.maxSpeed);
        if (login.maxSpeed > 0) {
            curl_easy_setopt(handler.get(), CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)(login.maxSpeed * 1024));
        }

        /// 是否使用断点续传
        file.seek(startPos);
        curl_easy_setopt(handler.get(), CURLOPT_APPEND, startPos > 0 ? 1L : 0L);

        CURLcode res = curl_easy_perform(handler.get());
        if (res != CURLE_OK) {
            DEBUG_LOG("curl_easy_perform() failed: " << curl_easy_strerror(res));
            if (res == CURLE_READ_ERROR)
            {
                emit errorOccured(tr("SD card error，please reformat or replace"));//
            }
            break;
        }

        status = true;
    } while (false);

    return status;
}

bool QTFtpInfo::AfterUpload(QString path)
{
    QString ftpConfigFile = m_SambaRoot + "/ftp_" + path + ".conf";
    remove(ftpConfigFile.toLocal8Bit().data());
    m_Progress->DirClear();
    return true;
}

bool QTFtpInfo::FtpServerFileSize(QString path, qint64& len)
{
    bool status = false;
    std::shared_ptr<CURL> handler(curl_easy_init(), curl_easy_cleanup);

    do {
        if (!handler) {
            DEBUG_LOG("easy_handle init error");
            break;
        }

        int ret = CURLE_OK;
        //QString url = "ftp://" + login.name + ":" + login.passwd + "@" + login.host + path;
        QString url = QString("ftp://") + login.host + ":" + QString::number(login.port) + path;
        QTFtpTools::convertSpecialChar(url);
        ret |= curl_easy_setopt(handler.get(), CURLOPT_URL, url.toLocal8Bit().data());
        ret |= curl_easy_setopt(handler.get(), CURLOPT_USERNAME, login.name.toLocal8Bit().data());
        ret |= curl_easy_setopt(handler.get(), CURLOPT_PASSWORD, login.passwd.toLocal8Bit().data());
        ret |= curl_easy_setopt(handler.get(), CURLOPT_HEADER, 1L);
        ret |= curl_easy_setopt(handler.get(), CURLOPT_NOBODY, 1L);
        if (ret != CURLE_OK) {
            break;
        }

        double size = 0; /// 目录不存在，perform执行失败;如果文件不存在，会被设置为-1
        ret = curl_easy_perform(handler.get());
        if (CURLE_OK != ret)
        {
            std::cout << "--get file size from ftp server failed! res is :" << ret << std::endl;
            status = true;
            if (CURLE_COULDNT_CONNECT == ret)
            {
                status = false;
            }
            else if (CURLE_REMOTE_ACCESS_DENIED == ret) { /// ftp文件夹或目录不存在
                size = 0;                
            }
            break;
        }

        ret = curl_easy_getinfo(handler.get(), CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
        if (CURLE_OK != ret) {
            break;
        }

        len = (qint64)size;
        if (len < 0) {
            len = 0;
        }
        status = true;
    } while (false);

    return status;
}

bool QTFtpInfo::InitSendFile(QString file, qint64 startPos)
{
    /// 初始化准备发送的文件信息
    m_Progress->FileClear();

    MD5_Init(&m_Md5Ctx);
    m_Progress->beginTime = time(NULL);
    m_Progress->dirName = QFileInfo(file).path();
    m_Progress->fileName = QFileInfo(m_SambaRoot + "/" + file).fileName();
    m_Progress->fileSize = QTFtpTools::GetFileSize(m_SambaRoot + "/" + file);
    m_Progress->fileBeforeSendSize = startPos;
    m_Progress->createTime = QFileInfo(m_SambaRoot + "/" + file).created().toString("yyyy-MM-dd hh:mm:ss");

    /// 计算md5值
    if (startPos > 0) {
        char line[1024] = { 0 };
        qint64 pos = 0;

        std::fstream fh;
        QString path = m_SambaRoot + "/" + file;
        try
        {
            fh.open(path.toStdString().c_str());

            while (fh.good() && pos < startPos) {
                int len = (startPos - pos) > 1024 ? 1024 : (startPos - pos);
                memset(line, 0x00, sizeof(line));
                fh.read(line, len);
                MD5_Update(&m_Md5Ctx, line, len);
                pos += len;
            }
            if (!fh.good())
            {
                emit errorOccured(tr("SD card error，please reformat or replace"));//SD卡错误，请重新格式化或更换
               DEBUG_LOG("read file error");
               return false;
            }
        }
        catch (...)
        {
            emit errorOccured(tr("SD card error，please reformat or replace"));//SD卡错误，请重新格式化或更换
            DEBUG_LOG("read file error");
            return false;
        }
        fh.close();
    }

    return true;
}

bool QTFtpInfo::EndSendFile(bool status)
{

    if (NULL != m_ProgressCB) {
        Progress progress;
        GetProgress(progress);
        MD5_Final(progress.md5Result, &m_Md5Ctx);
        progress.percentage = 100;
        m_ProgressCB(m_Ctx, progress);
    }

    m_Progress->dirSendSize += m_Progress->fileSize;

    m_Progress->FileClear();

    return true;
}

bool QTFtpInfo::IsNumeric(const char* cList)
{
    for (; *cList; cList++) {
        if (*cList < '0' || *cList > '9') {
            return false;
        }
    }
    return true;
}

pid_t QTFtpInfo::GetPIDByName(const char* processName)
{
    const char* PROC_DIRECTORY = "/proc/";
    pid_t pid = (pid_t)-1;
    std::shared_ptr<DIR> dirProc(opendir(PROC_DIRECTORY), closedir);

    if (!dirProc) {
        DEBUG_LOG("Couldn't open the [" << PROC_DIRECTORY << "] directory");
        return (pid_t)-2;
    }

    // Loop while not NULL
    struct dirent* de_DirEntity = NULL;

    while ((de_DirEntity = readdir(dirProc.get()))) {
        if (de_DirEntity->d_type != DT_DIR) {
            continue;
        }

        if (false == IsNumeric(de_DirEntity->d_name)) {
            continue;
        }

        char statusPath[256] = { 0 };
        strcpy(statusPath, PROC_DIRECTORY);
        strcat(statusPath, de_DirEntity->d_name);
        strcat(statusPath, "/status");
        std::shared_ptr<FILE> fdHandler(fopen(statusPath, "rt"), fclose);
        if (!fdHandler) {
            continue;
        }

        char nameOfProcess[300] = { 0 };
        fscanf(fdHandler.get(), "%s", nameOfProcess);
        fscanf(fdHandler.get(), "%s", nameOfProcess);
        fdHandler.reset();

        if (0 == strcasecmp(nameOfProcess, processName)) {
            pid = (pid_t)atoi(de_DirEntity->d_name);
            break;
        }
    }

    dirProc.reset();
    return pid;
}

bool QTFtpInfo::StartSmb(QString path)
{
    DEBUG_LOG(", param path is : " << path.toLocal8Bit().data());
    if (true == smbIsRun) {
        return true;
    }

    QDir tempDir;
    if (false == tempDir.exists(m_SambaRoot + "/ftp/sambaRootPath")) {
        if (false == tempDir.exists(m_SambaRoot + "/" + path)) {
            //tempDir.mkdir(m_SambaRoot + "/sambaRootPath");
            tempDir.mkpath(m_SambaRoot + "/ftp/sambaRootPath");
        } else {
            tempDir.rename(m_SambaRoot + "/" + path, m_SambaRoot + "/sambaRootPath");
        }
    }
    QString cmd = QString("chmod 777 ") + m_SambaRoot + "/ftp/sambaRootPath";
    system(cmd.toLocal8Bit().data());

    if (0 > GetPIDByName("vsftpd")) {
        //needn't smb.conf
//		std::ifstream sci;
//		sci.open("/etc/smb.conf-base");
//		remove("/tmp/smb.conf");
//		std::ofstream sco;
//		sco.open("/tmp/smb.conf");

//		char line[1024] = { 0 };
//		while (sci.good()) {
//			memset(line, 0x00, sizeof(line));
//			sci.getline(line, sizeof(line));
//			sco.write("\n", 1);
//			sco.write(line, strlen(line));
//		}

//		memset(line, 0x00, sizeof(line));
//		snprintf(line, sizeof(line) - 1, "   path =%s/%s", m_SambaRoot.toStdString().c_str(), "sambaRootPath");
//		sco.write(line, strlen(line));
//		sco.close();
//		sci.close();
        system("vsftpd &");
    }
    smbIsRun = true;

    return true;
}

static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    QTFtpInfo* ptr = (QTFtpInfo*)clientp;
    ptr->UpdateProgress(ulnow);
    return 0;
}

static size_t ReadCallBack(void* ptr, size_t size, size_t nmemb, void* stream)
{
    FtpTransfer* transfer = (FtpTransfer*)stream;
    qint64 len;

    if (NULL == transfer) {
        return -1;
    }
    len = transfer->fp->read((char*)ptr, size * nmemb);
    if (len < 0)
    {
        DEBUG_LOG("read file error");
        return -1;
    }

    MD5_Update(transfer->md5Ctx, ptr, len);

    return len;
}
