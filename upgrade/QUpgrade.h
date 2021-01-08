/*******************************************************************************
* Copyright(c) 2017
* All rights reserved.
*
* @filename QUpgrade.h
* @brief
* @history
* V1.0 2018/1/10
* @details
*******************************************************************************/
#ifndef _QUPGRADE_H_
#define _QUPGRADE_H_

#include <QtCore>
#include "curl/curl.h"
#include "enum.h"


/*!< 状态不需要原因 */
#define REASON_NEEDNOT              0

class WindowContext;

//升级状态
enum UPGRADE_STATE
{
    STATE_HASNOT_PLAN = 0,          /*!< 无升级计划 */
    STATE_HAS_PLAN,                 /*!< 有升级计划但未执行 */
    STATE_PACK_DOWNLOADING,         /*!< 正在下载升级包 */
    STATE_PACK_DOWNLOADED,          /*!< 下载已完成 */
    STATE_UPGRADING,                /*!< 正在更新 */
    STATE_FAILED,                   /*!< 升级失败*/
    STATE_SUCCESSED,                /*!< 更新成功*/
};
//升级失败原因
enum UPGRADE_FAILED_REASON
{
    FAILED_REASON_CANCEL = 1,       /*!< 用户取消 */
    FAILED_REASON_PARAMS,           /*!< 参数不正确 */
    FAILED_REASON_DOWNLOWD,         /*!< 下载失败 */
    FAILED_REASON_FILE,             /*!< 文件检查错误 */
    FAILED_REASON_VER_NEWEST,       /*!< 终端版本也是最新 */
    FAILED_REASON_VER_LOWER,        /*!< 终端版本不是适合升级到新版本*/
    FAILED_REASON_ANOTHER_PROC,     /*!< 另一个任务在下载 */
    FAILED_REASON_USING,            /*!< 仪器使用中*/
    FAILED_REASON_UNSYNC,           /*!< 仪器有未同步记录*/
    FAILED_REASON_UPDATE,           /*!< 更新失败*/
};

class QUpgrade : public QThread
{
Q_OBJECT
public:
    static bool checkVer(const QString ver);
    static bool compareVer(const QString ver1, QString ver2);
public:
    explicit QUpgrade(QObject* parent = NULL, WindowContext *context = 0);
    ~QUpgrade();
    bool startUpgrade(void);
    void stopUpgrade(void);
    bool isExecuting(void) const;
    bool isValidParms() const;
    QString getFailedReasonString(enum UPGRADE_FAILED_REASON reason) const;
    QString getDeviceVer(void);
    void getVer(QString *aNewVer, QString *aFromVer);
    void addUpgradeEvent(uint eventType);
    bool getNeedStop(void);
    void setState(enum UPGRADE_STATE aState)
    {
        state = aState;
    }
    enum UPGRADE_STATE getState(void)
    {
        return state;
    }
public slots:
    void onParamsChanged(void);
    void stopThread(void);
signals:
    void downloadProcess(int dltotal, int dlnow);                //下载进度信号
    void stateChange(enum UPGRADE_STATE, int reason);            //状态改变信号
protected:
    void run(void);
private slots:
    void onStateChangeForEvent(enum UPGRADE_STATE aState, int reason);
private:
    void checkUpgradeIsSuccess(void);
    bool downloadFile(void);
    bool upgradeSystem(void);
    void updateProgress(int dltotal, int dlnow);
    static size_t onWrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
    static size_t onProgress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
    void setCurlOption(CURL *curl, FILE *fp);
    bool getMac(QString *array) const;
    bool readUpgradeParams(void);
    QString getMac_ID_MD5(void) const;
    bool checkParamsPlan(void);
    bool checkDownloadedFile(void);
    quint32 calcAdler32(QFile *file);
    void emitStateChange(enum UPGRADE_STATE aState, int reason);

    /*!< 有效升级参数；true：升级参数是有效的。false：升级参数是无效的。*/
    bool enable;
    /*!< 强制更新标志,true：终端即使有未上传的使用记录，也执行更新；
     * false：若终端有未上传使用记录，则不允许更新*/
    bool forceUpgrade;
    /*!< 升级包的版本。版本格式必须为x.x.x。其中x为数字，例如：4.0.0*/
    QString newVer;
    /*!< 可以升级的终端程序最低版本const，低于此版本的终端程序不会被升级。
     * 版本格式必须为x.x.x。其中x为数字，例如：4.0.0*/
    QString fromVer;
    /*!< 升级包的长度，单位：字节*/
    unsigned int size;
    /*!< 升级包的adler32校验值 */
    unsigned int adler32;
    /*!< 存放升级包的地址。这个地址必须是HTTP服务器上的地址。*/
    QUrl url;
    /*!< id */
    unsigned long long upgradeId;

    enum UPGRADE_STATE state;
    WindowContext *windowContext;
    bool needStop;
};


#endif
