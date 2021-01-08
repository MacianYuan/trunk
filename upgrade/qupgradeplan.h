/*******************************************************************************
* Copyright(c) 2018
* All rights reserved.
*
* @filename qupgradeplan.h
* @brief
* @history
* V1.0 2018/2/8
* @details
*******************************************************************************/
#ifndef QUPGRADEPLAN_H
#define QUPGRADEPLAN_H

#include <QObject>
#include <QtCore>

class QUpgrade;

/*!< 升级计划最小间隔值，单位：分钟*/
#define MIN_PLAN_INTERVAL           120

/**
 * @brief 升级计划的类型
 */
typedef enum
{
    PLAN_TYPE_CANNEL = 0,       /*!< 取消计划 */
    PLAN_TYPE_TIMER  = 1,       /*!< 按设定的时间和时间间隔执行计划 */
    PLAN_TYPE_IMMIDIATE = 2,    /*!< 立即执行升级 */
} PLAN_TYPE;

class QUpgradePlan : public QObject
{
    Q_OBJECT
public:
    explicit QUpgradePlan(QUpgrade *aUpgrade, QObject *parent = nullptr);

signals:

public slots:
    void onPlanChanged(void);
private slots:
    void onPlanTimerTimeout(void);
private:
    bool readUpgradePlan(void);
    void immidateCheck(void);
private:
    /*!< 升级计划类型。0：取消计划；
     * 1：每隔“更新检查间隔时间x”检查，能否下载和更新。
     * 实际检查间隔时间是：更新检查间隔时间x+(仪器ID%120)*/
    int type;
    /*!< 升级开始时间。格式为：yyyy-M-d hh:mm:ss*/
    QDateTime beginTime;
    /*!< 升级检查间隔时间x。单位：分钟。例如：60，表示每小时检查能否升级*/
    int interval;

    QUpgrade *upgrade;
    QTimer planTimer;
    /*!< 到达升级计划时间后随机延迟此时间，才开始真正的*/
    int randomDelay;
};

#endif // QUPGRADEPLAN_H
