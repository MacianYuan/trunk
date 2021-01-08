//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_PUBLICDEFINE_H
#define OSLA_NEW_PUBLICDEFINE_H

class PublicDefine {
public:
    static int getReconnectInterval();

    static int getMaxRoundTripTime();

    static int getMinHeartbeatCount();

    static int getOfflineTimeOut();

    static int getSyncTimeOut();

    static int getForceRecheckInterval();

    static int getMaxSyncTryCount();

    static int getMaxListSize();

private:
    static int ReconnectInterval; // 自动重连间隔（秒）
    static int MaxRoundTripTime; // 允许的最大网络延迟（秒）
    static int MinHeartbeatCount; // 判断网络可靠所需最小连续心跳数
    static int OfflineTimeOut; // 判断离线的超时时间（秒）
    static int SyncTimeOut; // 同步超时时间（秒）
    static int ForceRecheckInterval; // 强制重新检查数据库间隔（秒）
    static int MaxSyncTryCount; // 最大同步尝试次数
    static int MaxListSize; // 单条消息允许包含最大记录数，超过此值需分多条消息发送
};


#endif //OSLA_NEW_PUBLICDEFINE_H
