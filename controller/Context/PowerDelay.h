
#ifndef POWERDELAY_H
#define POWERDELAY_H

#include <memory>
#include <QtCore/QObject>

class PowerDelay : public QObject
{
public:
    explicit PowerDelay(QObject* parent = NULL);
    ~PowerDelay();

    /// <summary>
    /// 启动的时候，初始化开关；如果设备不在使用，则关闭电源.
    /// </summary>
    void StartUpParse();

    /// <summary>
    /// 启动会话，检查上电保护.
    /// </summary>
    /// <returns>不在上电保护范围返回true，其它返回false.</returns>
    bool StartSesssion();

    /// <summary>
    /// 会话关闭，设置断点保护与上电保护.
    /// </summary>
    /// <returns>成功返回true，失败返回false.</returns>
    bool EndSession();

    /// <summary>
    /// 检测设备是否在工作.
    /// </summary>
    /// <param name="working">true:外接设备还在工作,false:外接设备不在工作.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool CheckInstrumentWorking(bool& working); 

    /// <summary>
    /// 控制电源开关.
    /// </summary>
    /// <param name="enabled">true:打开电源，false:关闭电源.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool SetInstrumentEnabled(bool enabled);

    /// <summary>
    /// 时间检测工作.
    /// </summary>
    /// <returns>成功返回true，失败返回false.</returns>
    bool RegularlyCheck();

private:
	int GetMinOffTime();
	int GetOffDelayTime();

private:
    bool needCheckStopPower{ false };               /// 是否需要检查设备电流
    uint powerOffDelay{ 0 };                        /// 下电保护，用户下线时设置，必须为0时，如果电流偏低，才可给设备断点
    uint powerOnDelay{ 0 };                         /// 上电保护，必须为0，才可以让用户上线，在用户下线的时候，设置此值
};

#endif //POWERDELAY_H