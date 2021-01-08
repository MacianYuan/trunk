/*******************************************************************************
* Copyright(c) 2017
* All rights reserved.
*
* @filename QUpgrade.cpp
* @brief
* @history
* V1.0 2018/1/10
* @details
*******************************************************************************/
#include <QtCore>
#include <QMessageBox>
#include <QSharedPointer>
#include <QMetaType>
#include <openssl/md5.h>
#include "QUpgrade.h"
#include "log.h"
#include "cmdparse/GlobalConfig.h"
#include "tools/IniParse.h"
#include "deviceinfo/version.h"
#include "Context/InstrumentStatus.h"
#include "quaadler32.h"
#include "controller/Context/WindowContext.h"
#include "Context/ServiceEvent.h"
#include "deviceinfo/enum.h"

//升级计划检查定时器启动时每50秒检查一次是否有升级计划要执行
//如果设置60秒，可能因为误差而不能保证每分钟都检查一次
#define PLAN_TIMER_CHECK_INTERVAL      (50 * 1000)

using namespace log4cplus;
using namespace log4cplus::helpers;

/**
 * @brief
 * @param
 * @return
 */
QUpgrade::QUpgrade(QObject *parent, WindowContext *context):
    QThread(parent)
{
    qRegisterMetaType<UPGRADE_STATE>("UPGRADE_STATE");

    adler32 = 0;
    enable = false;
    forceUpgrade = false;
    fromVer = "1.0.0";
    newVer = "1.0.0";
    size = 0;
    url = "";

    state = STATE_HASNOT_PLAN;
    windowContext = context;
    needStop = false;

    connect(this, SIGNAL(stateChange(UPGRADE_STATE,int)), this, SLOT(onStateChangeForEvent(UPGRADE_STATE,int)));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(stopThread()));

    if (!readUpgradeParams())
    {
        DEBUG_LOG("upgrade:init upgrade params error");
    }

    //检查是否升级成功
    checkUpgradeIsSuccess();
}

QUpgrade::~QUpgrade()
{
}

/**
 * @brief 检查版本号的格式是否正确
 * @param ver 版本号的格式为:x.x.x 其x必须为数字
 * @return
 */
bool QUpgrade::checkVer(const QString ver)
{
    QString str = "[0-9]+.[0-9]+.[0-9]+";
    QRegExp regExp(str);

    if (regExp.indexIn(ver) >= 0)
    {
        return true;
    }

    return false;
}

/**
 * @brief 比较两个版本字符串大小，
 * @param ver1
 * @param ver2
 * @return true:ver1 > ver2;false:ver <=ver2,或者格式错误
 */
bool QUpgrade::compareVer(const QString ver1, const QString ver2)
{
    if (!checkVer(ver1) || !checkVer(ver2))
    {
        return false;
    }

    QString str = "[0-9]+.[0-9]+.[0-9]+";
    QRegExp regExp(str);
    if (regExp.indexIn(ver1) < 0)
    {
        return false;
    }
    QString v1 = regExp.cap(0);
    if (regExp.indexIn(ver2) < 0)
    {
        return false;
    }
    QString v2 = regExp.cap(0);

    QStringList list;
    list = v1.split(".");
    int n1 = list.at(0).toInt() * 10000 + list.at(1).toInt() * 100 + list.at(2).toInt();
    list = v2.split(".");
    int n2 = list.at(0).toInt() * 10000 + list.at(1).toInt() * 100 + list.at(2).toInt();

    if (n1 >= n2)
    {
        return true;
    }

    return false;
}

/**
 * @brief 启动升级
 * @return
 */
bool QUpgrade::startUpgrade(void)
{
    if (!enable)
    {
        return false;
    }
    //判断是否有升级在执行
    if (isExecuting())
    {
        DEBUG_LOG("upgrade:other proc is executing");
        emit stateChange(STATE_FAILED, FAILED_REASON_ANOTHER_PROC);
        return false;
    }

    //通知升级进程进行下载
    DEBUG_LOG("upgrade:start download");
    state = STATE_PACK_DOWNLOADING;
    emit stateChange(state, REASON_NEEDNOT);

    return true;
}

/**
 * @brief 停止升级
 */
void QUpgrade::stopUpgrade()
{
    DEBUG_LOG("upgrade:stop upgrade");
    state = STATE_HASNOT_PLAN;
    emit stateChange(STATE_FAILED, FAILED_REASON_CANCEL);
}

/**
 * @brief 返回是否有升级任务在执行
 * @return true:有升级任务在执行；false:没有
 */
bool QUpgrade::isExecuting() const
{
    if ((state == STATE_PACK_DOWNLOADING) || (state == STATE_PACK_DOWNLOADED) || (state == STATE_UPGRADING))
    {
        return true;
    }

    return false;
}

/**
 * @brief 判断升级程序是否有效
 * @return
 */
bool QUpgrade::isValidParms() const
{
    if (enable)
    {
        return true;
    }

    return false;
}

/**
 * @brief 获得升级失败原因对应的文本解释
 * @return
 */
QString QUpgrade::getFailedReasonString(UPGRADE_FAILED_REASON reason) const
{
    struct REASON_TEXT
    {
        enum UPGRADE_FAILED_REASON reason;
        QString str;
    };
    REASON_TEXT reasonList[] =
    {
        {FAILED_REASON_CANCEL, tr("User cancelled")}, //用户取消
        {FAILED_REASON_PARAMS, tr("Incorrect upgrade paramenters")},//升级参数不正确
        {FAILED_REASON_DOWNLOWD, tr("Download upgrade file failed")},//下载升级文件失败
        {FAILED_REASON_FILE, tr("The upgrade file is incorrect")},//升级文件不正确
        {FAILED_REASON_VER_NEWEST, tr("The terminal version is already the latest version")},//终端版本已经是最新版本
        {FAILED_REASON_VER_LOWER, tr("The terminal version is too low to upgrade to a new version ")},//终端版本过低，不能升级到新版本
        {FAILED_REASON_ANOTHER_PROC, tr("The terminal is undergoing an upgrade")},//终端正在进行升级
        {FAILED_REASON_USING, tr("the instrument is in use")},//仪器正在使用中
        {FAILED_REASON_UNSYNC, tr("The terminal has unsynchornizes records")},//终端有未同步记录
        {FAILED_REASON_UPDATE, tr("failed to update terminal firmware")},//更新终端固件失败
    };

    for (int i = 0; i < sizeof(reasonList) / sizeof(reasonList[0]); i++)
    {
        if (reasonList[i].reason == reason)
        {
            return reasonList[i].str;
        }
    }

    return tr("Unknown cause");//未知的原因
}

/**
 * @brief 获得设备版本
 * @return
 */
QString QUpgrade::getDeviceVer()
{
    QString deviceVersion = QString::fromStdString(windowContext->getDeviceVersion()).mid(1);

    return deviceVersion;
}

/**
 * @brief 取得升级参数中的版本信息
 * @param newVer
 * @param fromVer
 */
void QUpgrade::getVer( QString *aNewVer, QString *aFromVer)
{
    *aNewVer = newVer;
    *aFromVer = fromVer;
}

/**
 * @brief 记录升级事件
 * @param event
 */
void QUpgrade::addUpgradeEvent(uint eventType)
{
    ServiceEvent::GetInstance()->AddSystemEvent(eventType, upgradeId);
}

bool QUpgrade::getNeedStop()
{
    return needStop;
}

/**
 * @brief 停止升级线程
 */
void QUpgrade::stopThread()
{
    needStop = true;
    DEBUG_LOG("upgrade:stop thread");
    //延时2秒，等线程退出
    sleep(2);
}

/**
 * @brief 下载进度更新
 * @param dltotal
 * @param dlnow
 */
void QUpgrade::updateProgress(int dltotal, int dlnow)
{
    emit downloadProcess(dltotal, dlnow);
}

/**
 * @brief 重新收到下发的升级参数和计划时，读取配置文件的内容
 */
void QUpgrade::onParamsChanged()
{
    //读取升级参数和升级计划
    if (!readUpgradeParams())
    {
        DEBUG_LOG("upgrade:upgrade params is error");
        emit stateChange(state, FAILED_REASON_PARAMS);
        return;
    }

    //检查升级升级参数和升级计划参数是否有效
    if (!checkParamsPlan())
    {
        DEBUG_LOG("upgrade:upgrade params is error");
        state = STATE_FAILED;
        emit stateChange(state, FAILED_REASON_PARAMS);
        return;
    }
}

/**
 * @brief 升级进程
 */
void QUpgrade::run()
{
    while (!needStop)
    {
        if (state == STATE_PACK_DOWNLOADING)
        {
            QString deviceVersion = getDeviceVer();

            if (compareVer(deviceVersion, newVer))
            {
                DEBUG_LOG("upgrade:ver is newest");
                emitStateChange(STATE_FAILED, FAILED_REASON_VER_NEWEST);
                continue;
            }

            if (!compareVer(deviceVersion, fromVer))
            {
                DEBUG_LOG("upgrade:ver is lower");
                emitStateChange(STATE_FAILED, FAILED_REASON_VER_LOWER);
                continue;
            }

            if (!downloadFile() && (state == STATE_PACK_DOWNLOADING))
            {   //下载失败
                DEBUG_LOG("upgrade:download failed");
                emitStateChange(STATE_FAILED, FAILED_REASON_DOWNLOWD);
                continue;
            }

            //检查是否在下载状态，如果不在下载状态，说明被停止了
            if (state == STATE_PACK_DOWNLOADING)
            {
                DEBUG_LOG("upgrade:emit STATE_PACK_DOWNLOADED");
                emitStateChange(STATE_PACK_DOWNLOADED, REASON_NEEDNOT);
                sleep(1);
                continue;
            }
        }
        if (state == STATE_PACK_DOWNLOADED)
        {
            //检查仪器是否在使用中
            if (windowContext->getInstrumentStatus()->GetInUseStatus())
            {
                DEBUG_LOG("upgrade:instrument is using");
                emitStateChange(STATE_FAILED, FAILED_REASON_USING);
                continue;
            }
            //检查是否有未同步记录
            else if ((!forceUpgrade) && (windowContext->getUnsyncCount() > 0))
            {
                DEBUG_LOG("upgrade:has unsync data");
                emitStateChange(STATE_FAILED, FAILED_REASON_UNSYNC);
                continue;
            }
            else
            {
                DEBUG_LOG("upgrade:updating");
                windowContext->setCardEnabled(false);
                emitStateChange(STATE_UPGRADING, REASON_NEEDNOT);

                //执行更新前先在文件中记录状态
                IniParse upgradeConfig(UPGRADE_CONFIG);
                upgradeConfig.Set("upgrade.state", STATE_UPGRADING);
                upgradeConfig.SaveFile();
                if (!upgradeSystem())
                {
                    DEBUG_LOG("upgrade:update failed");
                    upgradeConfig.Set("upgrade.state", STATE_FAILED);
                    upgradeConfig.SaveFile();
                    emitStateChange(STATE_FAILED, FAILED_REASON_UPDATE);
                }
                else
                {
                    windowContext->setCardEnabled(true);
                }
            }
        }

        sleep(1);
    }

    DEBUG_LOG("upgarde:exit QUpgrade thread");
}

/**
 * @brief 状态改变处理，在这里发送系统事件
 * @param aState
 * @param reason
 */
void QUpgrade::onStateChangeForEvent(UPGRADE_STATE aState, int reason)
{
    //状态和原因对应的事件
    struct STATE_FOR_EVENT
    {
        UPGRADE_STATE state;
        int reason;
        uint event;
    };
    struct STATE_FOR_EVENT stateForEvent[] =
    {
        {STATE_PACK_DOWNLOADED, 0, biotech::osla::SYSTEM_EVENT_UPGRADE_DOWNLOADED},
        {STATE_PACK_DOWNLOADING, 0, biotech::osla::SYSTEM_EVENT_UPGRADE_DOWNLOADING},
        {STATE_UPGRADING, 0, biotech::osla::SYSTEM_EVENT_UPGRADE_UPDATING},
        {STATE_SUCCESSED, 0, biotech::osla::SYSTEM_EVENT_UPGRADE_UPDATED},
        {STATE_FAILED, FAILED_REASON_CANCEL, biotech::osla::SYSTEM_EVENT_UPGRADE_USER_CANNEL},
        {STATE_FAILED, FAILED_REASON_PARAMS, biotech::osla::SYSTEM_EVENT_UPGRADE_PARAMS_ERR},
        {STATE_FAILED, FAILED_REASON_DOWNLOWD, biotech::osla::SYSTEM_EVENT_UPGRADE_DOWNLOAD_ERR},
        {STATE_FAILED, FAILED_REASON_FILE, biotech::osla::SYSTEM_EVENT_UPGRADE_FILE_ERR},
        {STATE_FAILED, FAILED_REASON_VER_LOWER, biotech::osla::SYSTEM_EVENT_UPGRADE_VER_LOWER},
        {STATE_FAILED, FAILED_REASON_VER_NEWEST, biotech::osla::SYSTEM_EVENT_UPGRADE_VER_NEWEST},
        {STATE_FAILED, FAILED_REASON_ANOTHER_PROC, biotech::osla::SYSTEM_EVENT_UPGRADE_ANOTHER_PROC},
        {STATE_FAILED, FAILED_REASON_USING, biotech::osla::SYSTEM_EVENT_UPGRADE_USING},
        {STATE_FAILED, FAILED_REASON_UNSYNC, biotech::osla::SYSTEM_EVENT_UPGRADE_UNSYNC},
        {STATE_FAILED, FAILED_REASON_UPDATE, biotech::osla::SYSTEM_EVENT_UPGRADE_UPDATE},
    };

    for (int i = 0; i < sizeof(stateForEvent) / sizeof(stateForEvent[0]); i++)
    {
        if ((stateForEvent[i].state == aState) && (stateForEvent[i].reason == reason))
        {
            addUpgradeEvent(stateForEvent[i].event);
            return;
        }
    }

    DEBUG_LOG("upgrade:unknown upgrade event, state:" << aState << ", reason:" << reason);
}

/**
 * @brief 判断升级是否成功。
 *        判断方法：
 *        OSLA.conf中upgrade.state为STATE_UPGRATING
 */
void QUpgrade::checkUpgradeIsSuccess()
{
    IniParse upgradeConfig(UPGRADE_CONFIG);

    if (upgradeConfig.GetLong("upgrade.state", 0) == STATE_UPGRADING)
    {
        DEBUG_LOG("upgrade: upgrade successed");
        upgradeConfig.Set("upgrade.state", STATE_SUCCESSED);
        upgradeConfig.SaveFile();
        state = STATE_SUCCESSED;
        emit stateChange(state, REASON_NEEDNOT);
    }
}

void QUpgrade::setCurlOption(CURL *curl, FILE *fp)
{
    QString urlString = url.toString();
    //urlString <= "?token=%1&insid=%2";
    //urlString = urlString.arg(getMac_ID_MD5()).arg(windowContext->getInstrumentStatus()->GetId());
    DEBUG_LOG("upgrade: download file from url:" << url.toString().toStdString());

    QByteArray array = urlString.toLocal8Bit();
    static char str[200];
    strcpy(str, array.constData());
    curl_easy_setopt(curl, CURLOPT_URL, str); /// 设置url
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); //详细输出curl信息，用于调试
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWrite); /// 设置写数据回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); /// 设置写数据回调函数的参数
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, onProgress);//进度响应回调函数
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);    //设置为0,才能设置进度响应函数
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);//设置数据传输回调函数的参数
    // 跳过服务器SSL验证，不使用CA证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // 验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,1000); //设置下载超时
}

/**
 * @brief 下载写入文件的回调函数
 * @param ptr
 * @param size
 * @param nmemb
 * @param stream
 * @return
 */
size_t QUpgrade::onWrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

/**
 * @brief 下载进度响应函数
 * @param clientp
 * @param dltotal
 * @param dlnow
 * @param ultotal
 * @param ulnow
 * @return
 */
size_t QUpgrade::onProgress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    QUpgrade* ptr = (QUpgrade*)clientp;

    //检查是否终止下载
    if ((STATE_PACK_DOWNLOADING != ptr->state) || (ptr->getNeedStop()))
    {
        DEBUG_LOG("upgrade:download canceled");
        return -1;
    }

    ptr->updateProgress(dltotal, dlnow);

    return 0;
}

/**
 * @brief 获得机器MAC地址
 * @param array 保存得到的MAC地址
 * @return true:成功获得
 */
bool QUpgrade::getMac(QString *array) const
{
    QFile netConfigFile("/proc/cmdline");
    if (netConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netConfigFile);
        QString StringMac = stream.readAll();
        int MAC = StringMac.indexOf(QRegExp("mac="), 0);
        QString tMac = StringMac.mid(MAC + 4, 17);
        array->clear();
        array->append(tMac);
    } else {
        ERROR_LOG("Failed to open mac of cmdline");
        return false;
    }
    netConfigFile.close();

    return true;
}

/**
 * @brief 读取配置文件的升级参数
 */
bool QUpgrade::readUpgradeParams()
{
    DEBUG_LOG("upgrade:read Upgrade Params");
    IniParse upgradeConfig(UPGRADE_CONFIG);

    enable = upgradeConfig.GetBool("upgradeParams.enable", false);
    forceUpgrade = upgradeConfig.GetBool("upgradeParams.forceUpgrade", false);
    newVer = QString::fromStdString(upgradeConfig.GetString("upgradeParams.newVer", ""));
    fromVer = QString::fromStdString(upgradeConfig.GetString("upgradeParams.fromVer", ""));
    QString str = QString::fromStdString(upgradeConfig.GetString("upgradeParams.size", "0"));
    bool ok;
    size = str.toULongLong(&ok);
    if (!ok)
    {
        size = 0;
    }
    str = QString::fromStdString(upgradeConfig.GetString("upgradeParams.adler32", "0"));
    adler32 = str.toULongLong(&ok);
    if (!ok)
    {
        adler32 = 0;
    }
    url = QString::fromStdString(upgradeConfig.GetString("upgradeParams.url", ""));
    str = QString::fromStdString(upgradeConfig.GetString("upgradeParams.upgradeId", "0"));
    upgradeId = str.toULongLong(&ok);
    if (!ok)
    {
        upgradeId = 0;
    }

    int temp = upgradeConfig.GetLong("upgrade.state", 0);
    if (temp == (int)STATE_HAS_PLAN)
    {
        state = STATE_HAS_PLAN;
    }
    else
    {
        state = STATE_HASNOT_PLAN;
    }

    return true;
}

/**
 * @brief 从服务器下载文件
 * @return
 */
bool QUpgrade::downloadFile()
{
    QFileInfo fileInfo("/storage/sd0/upgrade");

    if (!fileInfo.exists())
    {
        QDir dir;
        DEBUG_LOG("create /storage/sd0/upgrade");
        dir.mkdir("/storage/sd0/upgrade");
    }

    std::shared_ptr<CURL> cp(curl_easy_init(), curl_easy_cleanup);
    std::shared_ptr<FILE> fp(fopen(UPGRADE_FILE_NAME, "w+"), fclose);

    if (!fp || !cp)
    {
        ERROR_LOG("upgrade:other proc is executing");
        return false;
    }

    setCurlOption(cp.get(), fp.get());
    CURLcode res = curl_easy_perform(cp.get());
    if (res != CURLE_OK)
    {
        DEBUG_LOG("upgrade: curl_easy_perform() failed: " << curl_easy_strerror(res));
        return false;
    }

    DEBUG_LOG("upgrade:download finished");
    return true;
}

/**
 * @brief 执行更新
 * @return
 */
bool QUpgrade::upgradeSystem()
{
    QDir dir(UPGRADE_UNCOMPRESS_PATH);
    QProcess process;
    QString cmd;

    if (!checkDownloadedFile())
    {
        DEBUG_LOG("upgrade:upgrade file error");
        emitStateChange(STATE_FAILED, FAILED_REASON_FILE);
        return false;
    }

    //删除目录,重新创建
    cmd = "rm -f -r ";
    cmd += dir.absolutePath();
    if (process.execute(cmd) < 0)
    {
        ERROR_LOG("upgrade:delete upgrade dest dir error");
    }
    cmd = "mkdir ";
    cmd += dir.absolutePath();
    if (process.execute(cmd) < 0)
    {
        ERROR_LOG("upgrade:mkdir upgrade dest dir error");
        return false;
    }

    cmd = "busybox tar -xf ";
    cmd += UPGRADE_FILE_NAME;
    cmd += " -C ";
    cmd += UPGRADE_UNCOMPRESS_PATH;
    if (process.execute(cmd) < 0)
    {
        ERROR_LOG("upgrade:tar file error");
        state = STATE_FAILED;
        emit stateChange(state, FAILED_REASON_FILE);
        return false;
    }
    process.setWorkingDirectory(UPGRADE_UNCOMPRESS_PATH);
    if (process.execute(UPDATE_SH) < 0)
    {
        ERROR_LOG("upgrade:update.sh error");
        state = STATE_FAILED;
        emit stateChange(state, FAILED_REASON_FILE);
        return false;
    }

    //正常情况下，执行到此时系统可能已不能正常运行，进行重启系统
    process.execute("reboot");

    return true;
}

/**
 * @brief 计算mac+仪器id的md5值
 * @param md5Result
 */
QString QUpgrade::getMac_ID_MD5(void) const
{
    QString md5;
    QString mac;

    if (!getMac(&mac))
    {
        ERROR_LOG("upgrade: get mac error!");
        return md5;
    }

    int id = windowContext->getInstrumentStatus()->GetId();
    QString str = QString("%1").arg(id, 11);

    md5 = mac + str;

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, md5.toLocal8Bit().data(), md5.size());
   char md5Result[16];
    MD5_Final((unsigned char*)md5Result, &ctx);
    md5 = QString::fromLatin1(md5Result, 16);

    return md5;
}

/**
 * @brief 检查升级参数和升级的有效性
 * @return
 */
bool QUpgrade::checkParamsPlan()
{
    if (enable == false)
    {
        return false;
    }
    if (!checkVer(newVer) || !checkVer(fromVer))
    {
        return false;
    }
    if (!url.isValid())
    {
        return false;
    }
    if (size == 0)
    {
        return false;
    }

    return true;
}

/**
 * @brief 检查下的文件是否有效
 * @return
 */
bool QUpgrade::checkDownloadedFile()
{
    QFile file(UPGRADE_FILE_NAME);

    unsigned int checkValue = calcAdler32(&file);

    qDebug() << checkValue;

    if (!file.exists() || (checkValue != adler32)
            || (file.size() != size))
    {
        return false;
    }

    return true;
}

/**
 * @brief 计算文件的adler32值
 * @param file
 * @return
 */
quint32 QUpgrade::calcAdler32(QFile *file)
{
    if (!file->exists())
    {
        return 0;
    }

    if (!file->open(QIODevice::ReadOnly))
    {
        return 0;
    }

    QuaAdler32 adler;

    adler.reset();
    QByteArray array;
    while (!file->atEnd())
    {
         array = file->read(1024);
         if (array.isEmpty())
         {
             break;
         }
         adler.update(array);
    }

    return adler.value();
}

/**
 * @brief 发射stateChange信号，给run()线程使用
 * @param aState
 * @param reason
 * @return
 */
void QUpgrade::emitStateChange(UPGRADE_STATE aState, int reason)
{
    state = aState;
    emit stateChange(state, reason);
}
