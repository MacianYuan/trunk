//
// Created by 何继胜 on 2017/5/2.
//

#ifndef OSLA_NEW_TABLEBASEINFO_H
#define OSLA_NEW_TABLEBASEINFO_H

struct TableDefine {
    char *name;
    char *def;
};

TableDefine tableDefineArr[] = {
        /// 用户信息
        {
                "TUser",            "create table TUser(ID INTEGER PRIMARY KEY,Status SMALLINT,FirstName TEXT,"
                                            "LastName TEXT,Phone TEXT,CardSerial TEXT,AccountID INT,"
                                            "IsInBlack INT, PenaltyStopStart int, PenaltyStopEnd int, UpdateTime INT default (strftime('%s','now')))"},
        // 账户表
        {
                "TAccount",         "create table TAccount(ID INTEGER PRIMARY KEY,Status SMALLINT,GroupID INT,"
                                            "Deposit INT,Credit INT,CreditTemp INT,CreditTempExpireTime INT,"
                                            "UpdateTime INT default (strftime('%s','now')))"
        },

        // 用户资格表
        {
                "TUserRight",       "create table TUserRight(UserID INTEGER PRIMARY KEY,Right SMALLINT,RightTemp SMALLINT,"
                                            "RightTempExpireTime INT,UpdateTime INT default (strftime('%s','now')))"
        },

        // 临时卡关联表
        {
                "TTempCardBinding", "create table TTempCardBinding(CardSerial TEXT PRIMARY KEY,UserID INT,"
                                            "ActiveTime INT,ExpireTime INT,UpdateTime INT default (strftime('%s','now')))"
        },

        // 仪器专享时间表
        {
                "TReservation",     "create table TReservation(ID INTEGER PRIMARY KEY,GroupID INT,Week SMALLINT,"
                                            "StartTime INT,EndTime INT,ActiveTime INT,ExpireTime INT,"
                                            "UpdateTime INT default (strftime('%s','now')))"
        },

        // 预约记录
        {
                "TBooking",         "create table TBooking(ID INTEGER PRIMARY KEY,UserID INT,StartTime INT,"
                                            "EndTime INT,Type SMALLINT,Status SMALLINT,UpdateTime INT "
                                            "default (strftime('%s','now')),EndFlag BOOLEAN default 0)"
        },

        // 日历表
        {
                "TCalendar",        "create table TCalendar(Day INTEGER PRIMARY KEY,IsHoliday BOOLEAN,UpdateTime INT "
                                            "default (strftime('%s','now')))"
        },

        // 使用记录表
        {
                "TSession",         "create table TSession(ID INTEGER PRIMARY KEY,StartTime INT,EndTime INT,"
                                            "Type SMALLINT,UserID INT,CardSerial TEXT,UserRight SMALLINT,"
                                            "LongTimeUserRight SMALLINT, AccountID INT,"
                                            "GroupID INT,BookingID int,PenaltyFlag BOOLEAN default 0,CompleteFlag BOOLEAN default 0,"
                                            "SyncTime INT default 0,SyncFlag int default 0)"
        },

        // 培训用户表
        {
                "TUserInTraining",  "create table TUserInTraining(SessionID INTEGER,CardSerial TEXT,"
                                            "UserID INT,CheckInTime INT,CompleteFlag BOOLEAN default 0,"
                                            "SyncTime INT default 0,SyncFlag int default 0,PRIMARY KEY(SessionID, CardSerial))"
        },

        // 用户事件表
        {
                "TUserEvent",       "create table TUserEvent(LocalId INTEGER PRIMARY KEY,ID INTEGER,UserID INT,"\
                                "GroupID INT,CardSerial TEXT,CardType SMALLINT,ActionType SMALLINT,"\
                                "ActionResult SMALLINT,CreateTime INT,SyncTime INT default 0,"\
                                "SyncFlag int default 0)"
        },

        // 系统事件表
        {
                "TSystemEvent",     "create table TSystemEvent(LocalId INTEGER PRIMARY KEY,ID INTEGER,EventType SMALLINT,"
                                            "EventContent INT,CreateTime INT,SyncTime INT default 0,SyncFlag int default 0)"
        },

        // FTP事件
        {
                "TFtpEvent",        "create table TFtpEvent (ID INTEGER PRIMARY KEY,EventId int,UserId int,"
                                            "InstrumentId int,GroupId int,ActivedAccount int,FileName TEXT,"
                                            "FilePath Text,ConsumeTime int,FIleSize int,CreateTime Text,"
                                            "Percentage int default 0,SyncTime int default 0,SyncFlag int default 0)"
        },

        ///insert into TFtpEvent(EventId, UserId, InstrumentId, GroupId, ActivedAccount, FileName, FilePath, ConsumeTime, Percentage) values (111, 222,        8,             12,       12,          '123', '222', 0, 100);

        // 枚举文本表
        {
                "TEnumText",        "create table TEnumText(ID INTEGER PRIMARY KEY, Text TEXT)"
        },

        // 采集器采集的数据
        {
                "TDeviceCaptureEvent",
                                    "create table TDeviceCaptureEvent("\
                    "ID INTEGER PRIMARY KEY, "\
                    "DeviceName TEXT, "\
                    "DeviceValue TEXT, "\
                    "createTimer int, "\
                    "SyncTime int default 0, "\
                    "SyncFlag int default 0)"
        },

        // 委托实验
        {
                "TBookingEntrust",
                                    "create table TBookingEntrust("\
                     "ID INTEGER PRIMARY KEY, "\
                     "UserID INTEGER, "\
                     "Title Text, "\
                     "Status Integer, "\
                     "IsInExperiment Integer default 0"\
                ")"
        },

        // 委托实验记录
        {
                "TBookingEntrustEvent",
                                    "create table TBookingEntrustEvent("\
                     "ID INTEGER PRIMARY KEY,"\
                     "EntrustID INTEGER, "\
                     "Status Integer, "\
                     "IsInExperiment Integer,"\
                     "SessionId Integer not null," \
                     "CreateTime INT not null,"\
                     "SyncTime INT default 0,"\
                     "SyncFlag int default 0"\
                ")"
        },
        {       nullptr,            nullptr}
};

char *enumDefineArr[] = {
        "replace into TEnumText(ID,Text) values(201,'培训')",
        "replace into TEnumText(ID,Text) values(202,'维护')",
        "replace into TEnumText(ID,Text) values(203,'使用')",
        "replace into TEnumText(ID,Text) values(204,'代刷')",
        "replace into TEnumText(ID,Text) values(205,'教学')",
        "replace into TEnumText(ID,Text) values(206,'项目委托')",

        "replace into TEnumText(ID,Text) values(331,'管理员卡')",
        "replace into TEnumText(ID,Text) values(332,'用户卡')",
        "replace into TEnumText(ID,Text) values(333,'临时卡')",
        "replace into TEnumText(ID,Text) values(334,'未知卡')",

        "replace into TEnumText(ID,Text) values(1401,'无操作')",
        "replace into TEnumText(ID,Text) values(1402,'实验开机')",
        "replace into TEnumText(ID,Text) values(1403,'培训开机')",
        "replace into TEnumText(ID,Text) values(1404,'维护开机')",
        "replace into TEnumText(ID,Text) values(1405,'代刷开机')",
        "replace into TEnumText(ID,Text) values(1406,'培训签到')",
        "replace into TEnumText(ID,Text) values(1407,'关机')",
        "replace into TEnumText(ID,Text) values(1408,'系统设置')",
        "replace into TEnumText(ID,Text) values(1409,'教学开机')",
        "replace into TEnumText(ID,Text) values(1410,'项目委托开机')",
        "replace into TEnumText(ID,Text) values(1411,'教学签到')",

        "replace into TEnumText(ID,Text) values(1421,'无操作')",
        "replace into TEnumText(ID,Text) values(1422,'成功')",
        "replace into TEnumText(ID,Text) values(1423,'失败:未知用户')",
        "replace into TEnumText(ID,Text) values(1424,'失败:临时卡过期')",
        "replace into TEnumText(ID,Text) values(1425,'失败:仪器故障')",
        "replace into TEnumText(ID,Text) values(1426,'失败:仪器未启用')",
        "replace into TEnumText(ID,Text) values(1427,'失败:仪器报废')",
        "replace into TEnumText(ID,Text) values(1428,'失败:非开机者')",
        "replace into TEnumText(ID,Text) values(1429,'失败:预约锁定')",
        "replace into TEnumText(ID,Text) values(1430,'失败:专享锁定')",
        "replace into TEnumText(ID,Text) values(1431,'失败:资格不符')",
        "replace into TEnumText(ID,Text) values(1432,'失败:无付款账户')",
        "replace into TEnumText(ID,Text) values(1433,'失败:账户无效')",
        "replace into TEnumText(ID,Text) values(1434,'失败:账户余额不足')",
        "replace into TEnumText(ID,Text) values(1435,'失败:签到结束')",
        "replace into TEnumText(ID,Text) values(1436,'失败:重复签到')",

        "replace into TEnumText(ID,Text) values(1450,'仪器设为启用')",
        "replace into TEnumText(ID,Text) values(1451,'仪器设为故障')",
        "replace into TEnumText(ID,Text) values(1452,'仪器设为未启用')",
        "replace into TEnumText(ID,Text) values(1453,'仪器设为报废')",
        "replace into TEnumText(ID,Text) values(1454,'更改网络设置')",
        "replace into TEnumText(ID,Text) values(1455,'解除仪器绑定')",
        "replace into TEnumText(ID,Text) values(1456,'解除仪器绑定失败')",

        "replace into TEnumText(ID,Text) values(1471,'初始化完成')",
        "replace into TEnumText(ID,Text) values(1472,'恢复出厂设置')",
        "replace into TEnumText(ID,Text) values(1473,'冷启动')",
        "replace into TEnumText(ID,Text) values(1474,'热启动')",
        "replace into TEnumText(ID,Text) values(1475,'状态变更')",
        "replace into TEnumText(ID,Text) values(1476,'版本更新')",

        "replace into TEnumText(ID,Text) values(1481,'预约完成')",
        "replace into TEnumText(ID,Text) values(1482,'预约过期')",
        "replace into TEnumText(ID,Text) values(1483,'预约被占用')",
        "replace into TEnumText(ID,Text) values(1484,'预约终止')",
        "replace into TEnumText(ID,Text) values(1485,'预约锁定开始')",
        "replace into TEnumText(ID,Text) values(1486,'预约锁定开始')",
        "replace into TEnumText(ID,Text) values(1487,'预约锁定开始')",

        "replace into TEnumText(ID,Text) values(1491,'使用时间超长')",

        "replace into TEnumText(ID,Text) values(5000,'执行手工升级')",
        "replace into TEnumText(ID,Text) values(5001,'执行远程升级')",
        "replace into TEnumText(ID,Text) values(5002,'执行升级计划')",
        "replace into TEnumText(ID,Text) values(5003,'开始下载')",
        "replace into TEnumText(ID,Text) values(5004,'下载完成')",
        "replace into TEnumText(ID,Text) values(5005,'正在更新')",
        "replace into TEnumText(ID,Text) values(5006,'更新成功')",
        "replace into TEnumText(ID,Text) values(5007,'更新失败-用户取消')",
        "replace into TEnumText(ID,Text) values(5008,'更新失败-升级参数不正确')",
        "replace into TEnumText(ID,Text) values(5009,'更新失败-下载失败')",
        "replace into TEnumText(ID,Text) values(5010,'更新失败-文件检查错误')",
        "replace into TEnumText(ID,Text) values(5011,'更新失败-终端已经是最新版本')",
        "replace into TEnumText(ID,Text) values(5012,'更新失败-终端版本过低')",
        "replace into TEnumText(ID,Text) values(5013,'更新失败-另一个升级任务正在执行')",
        "replace into TEnumText(ID,Text) values(5014,'更新失败-仪器正在使用中')",
        "replace into TEnumText(ID,Text) values(5015,'更新失败-仪器有未同步记录')",
        "replace into TEnumText(ID,Text) values(5016,'更新失败-刷新终端程序时出错')",

        nullptr,
};

char *enumDefineArr_en[] = {
        "replace into TEnumText(ID,Text) values(201,'Training')",
        "replace into TEnumText(ID,Text) values(202,'Maintenance')",
        "replace into TEnumText(ID,Text) values(203,'In use')",
        "replace into TEnumText(ID,Text) values(204,'By admin')",
        "replace into TEnumText(ID,Text) values(205,'Education')",
        "replace into TEnumText(ID,Text) values(206,'Entrust')",

        "replace into TEnumText(ID,Text) values(331,'Admin card')",
        "replace into TEnumText(ID,Text) values(332,'User card')",
        "replace into TEnumText(ID,Text) values(333,'T-card')",
        "replace into TEnumText(ID,Text) values(334,'Unknown card')",

        "replace into TEnumText(ID,Text) values(1401,'NOP')",
        "replace into TEnumText(ID,Text) values(1402,'Experiment on')",
        "replace into TEnumText(ID,Text) values(1403,'Start Training')",
        "replace into TEnumText(ID,Text) values(1404,'Start Maintenance')",
        "replace into TEnumText(ID,Text) values(1405,'Start by admin')",
        "replace into TEnumText(ID,Text) values(1406,'Training Sign in')",
        "replace into TEnumText(ID,Text) values(1407,'End')",
        "replace into TEnumText(ID,Text) values(1408,'Setting')",
        "replace into TEnumText(ID,Text) values(1409,'Start Education')",
        "replace into TEnumText(ID,Text) values(1410,'Start Entrust')",
        "replace into TEnumText(ID,Text) values(1411,'Education Sign in')",

        "replace into TEnumText(ID,Text) values(1421,'NOP')",
        "replace into TEnumText(ID,Text) values(1422,'Succeed')",
        "replace into TEnumText(ID,Text) values(1423,'Failed:Unknown user')",
        "replace into TEnumText(ID,Text) values(1424,'Failed:T-card expired')",
        "replace into TEnumText(ID,Text) values(1425,'Failed:Instrument failure')",
        "replace into TEnumText(ID,Text) values(1426,'Failed:Instrument disable')",
        "replace into TEnumText(ID,Text) values(1427,'Failed:Instrument scrap')",
        "replace into TEnumText(ID,Text) values(1428,'Failed:No starter')",
        "replace into TEnumText(ID,Text) values(1429,'Failed:Appointment lock')",
        "replace into TEnumText(ID,Text) values(1430,'Failed:Exclusive lock')",
        "replace into TEnumText(ID,Text) values(1431,'Failed:Unqualified')",
        "replace into TEnumText(ID,Text) values(1432,'Failed:No payment account')",
        "replace into TEnumText(ID,Text) values(1433,'Failed:Invalid account')",
        "replace into TEnumText(ID,Text) values(1434,'Failed:Insufficient account')",
        "replace into TEnumText(ID,Text) values(1435,'Failed:Sign in End')",
        "replace into TEnumText(ID,Text) values(1436,'Failed:Sign in Repeat')",

        "replace into TEnumText(ID,Text) values(1450,'Instrument enabled')",
        "replace into TEnumText(ID,Text) values(1451,'Instrument fault')",
        "replace into TEnumText(ID,Text) values(1452,'Instrument disable')",
        "replace into TEnumText(ID,Text) values(1453,'Instrument scrap')",
        "replace into TEnumText(ID,Text) values(1454,'Change network Setting')",
        "replace into TEnumText(ID,Text) values(1455,'Unbind instrument')",
        "replace into TEnumText(ID,Text) values(1456,'Unbind instrument failed')",

        "replace into TEnumText(ID,Text) values(1471,'Initialization complete')",
        "replace into TEnumText(ID,Text) values(1472,'Restore factory setting')",
        "replace into TEnumText(ID,Text) values(1473,'Cold boot')",
        "replace into TEnumText(ID,Text) values(1474,'Warm boot')",
        "replace into TEnumText(ID,Text) values(1475,'Status change')",
        "replace into TEnumText(ID,Text) values(1476,'Version updating')",

        "replace into TEnumText(ID,Text) values(1481,'Appointment complete')",
        "replace into TEnumText(ID,Text) values(1482,'Appointment overdue')",
        "replace into TEnumText(ID,Text) values(1483,'Appointment occupied')",
        "replace into TEnumText(ID,Text) values(1484,'Appointment termination')",
        "replace into TEnumText(ID,Text) values(1485,'Appointment lock start')",
        "replace into TEnumText(ID,Text) values(1486,'Appointment lock start')",
        "replace into TEnumText(ID,Text) values(1487,'Appointment lock start')",

        "replace into TEnumText(ID,Text) values(1491,'Use for a long time')",

        "replace into TEnumText(ID,Text) values(5000,'Manual upgrades')",
        "replace into TEnumText(ID,Text) values(5001,'Remote upgrades')",
        "replace into TEnumText(ID,Text) values(5002,'Upgrading plan')",
        "replace into TEnumText(ID,Text) values(5003,'Start the download')",
        "replace into TEnumText(ID,Text) values(5004,'Download completes')",
        "replace into TEnumText(ID,Text) values(5005,'Updating')",
        "replace into TEnumText(ID,Text) values(5006,'Update successfully')",
        "replace into TEnumText(ID,Text) values(5007,'Update failed-User cancelled')",
        "replace into TEnumText(ID,Text) values(5008,'Update failed-Incorrect upgrade paramenters')",
        "replace into TEnumText(ID,Text) values(5009,'Update failed-Download failed')",
        "replace into TEnumText(ID,Text) values(5010,'Update failed-File check error')",
        "replace into TEnumText(ID,Text) values(5011,'Update failed-Up to date')",
        "replace into TEnumText(ID,Text) values(5012,'Update failed-Version is too low')",
        "replace into TEnumText(ID,Text) values(5013,'Update failed-Another upgrade task is in progress')",
        "replace into TEnumText(ID,Text) values(5014,'Update failed-Instrument in use')",
        "replace into TEnumText(ID,Text) values(5015,'Update failed-Instrument is not syncchronized')",
        "replace into TEnumText(ID,Text) values(5016,'Update failed-Error:Upgrading')",

        nullptr,
};
#endif //OSLA_NEW_TABLEBASEINFO_H
