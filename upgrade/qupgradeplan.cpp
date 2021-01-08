/*******************************************************************************
* Copyright(c) 2018
* All rights reserved.
*
* @filename qupgradeplan.cpp
* @brief
* @history
* V1.0 2018/2/8
* @details
*******************************************************************************/
#include <QtCore>
#include "log.h"
#include "deviceinfo/enum.h"
#include "tools/IniParse.h"
#include "deviceinfo/version.h"
#include "deviceinfo/enum.h"
#include "QUpgrade.h"
#include "qupgradeplan.h"

//升级计划检查定时器启动时每50秒检查一次是否有升级计划要执行
#define PLAN_TIMER_CHECK_INTERVAL      (50 * 1000)
//到达计划时间后，再延迟时，最长的延迟时间
#define RAND_DELAY_MAX                  30

using namespace log4cplus;
using namespace log4cplus::helpers;

QUpgradePlan::QUpgradePlan(QUpgrade *aUpgrade, QObject *parent) : QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    randomDelay =  qrand() % RAND_DELAY_MAX;
    upgrade = aUpgrade;
    readUpgradePlan();

    connect(&planTimer, SIGNAL(timeout()), this, SLOT(onPlanTimerTimeout()));
    planTimer.setSingleShot(false);

    planTimer.start(PLAN_TIMER_CHECK_INTERVAL);
}

/**
 * @brief 定时检查计划任务
 */
void QUpgradePlan::onPlanTimerTimeout()
{
    DEBUG_LOG("upgrade: check upgarde plan");
    if ((!upgrade->isValidParms()) || (type != PLAN_TYPE_TIMER))
    {

        return;
    }

    if ((upgrade->getState() != STATE_HAS_PLAN) && !(upgrade->getState() == STATE_FAILED))
    {
        return;
    }

    DEBUG_LOG("upgrade:check excuting");
    if (upgrade->isExecuting())
    {
        return;
    }

    DEBUG_LOG("upgrade:check plan start time");
    //判断是否到达升级时间
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000 / 60;  //转换成分钟
    qint64 begin = beginTime.toMSecsSinceEpoch() / 1000 / 60 + randomDelay;
    if (currentTime < begin)
    {
        return;
    }
    DEBUG_LOG("upgrade:check plan reach time");
    if (((currentTime - begin) % interval) != 0)
    {
        return;
    }

    DEBUG_LOG("upgrade: plan executing");
    upgrade->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_PLAN);
    upgrade->startUpgrade();
}

/**
 * @brief 服务器下发新升级计划时，从配置重新读取升级计划
 */
void QUpgradePlan::onPlanChanged()
{
    readUpgradePlan();
}

/**
 * @brief 读取升级计划
 * @return
 */
bool QUpgradePlan::readUpgradePlan()
{
    DEBUG_LOG("upgrade:read Upgrade plan");

    if ((upgrade->getState() == STATE_PACK_DOWNLOADING) || (upgrade->getState() == STATE_PACK_DOWNLOADED) || (upgrade->getState() == STATE_UPGRADING))
    {
        DEBUG_LOG("upgrade: can't execute, state:" << upgrade->getState());
        return true;
    }

    IniParse upgradeConfig(UPGRADE_CONFIG);

    type = upgradeConfig.GetLong("upgradePlan.type", 0);
    QString str = QString::fromStdString(upgradeConfig.GetString("upgradePlan.start_date", ""));
    beginTime = QDateTime::fromString(str, "yyyy-M-d h:m:s");
    interval = upgradeConfig.GetLong("upgradePlan.interval", 120);     //默认值是120分钟
    int state = upgradeConfig.GetLong("upgrade.state", 0);
    if (state == STATE_HAS_PLAN)
    {
        upgrade->setState(STATE_HAS_PLAN);
    }
    else
    {
        upgrade->setState(STATE_HASNOT_PLAN);
    }
    immidateCheck();

    return true;
}

/**
 * 检查是不是立即升级，如果是，则执行立即升级
 */
void QUpgradePlan::immidateCheck()
{
    if ((!upgrade->isValidParms()) || (type != PLAN_TYPE_IMMIDIATE))
    {
        return;
    }

    DEBUG_LOG("upgrade: immidate executing");
    upgrade->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_REMOTE);

    //立即升级只执行一次
    IniParse upgradeConfig(UPGRADE_CONFIG);
    upgradeConfig.Set("upgradePlan.type", 0);
    upgradeConfig.SaveFile();

    if (upgrade->isExecuting())
    {
        upgrade->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_ANOTHER_PROC);
        return;
    }

    upgrade->startUpgrade();
}
