//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_TOOLS_H
#define OSLA_NEW_TOOLS_H

class Tools {
public:
    static const int ReconnectInterval = 15; // 自动重连间隔（秒）
    static const int HeartbeatInterval = 30; // 心跳间隔（秒）
    static const int MaxRoundTripTime = 120; // 允许的最大网络延迟（秒）
    static const int MinHeartbeatCount = 3; // 判断网络可靠所需最小连续心跳数
    static const int OfflineTimeOut = 120; // 判断离线的超时时间（秒）
    static const int SyncTimeOut = 15; // 同步超时时间（秒）
    static const int ForceRecheckInterval = 3600; // 强制重新检查数据库间隔（秒）
    static const int MaxSyncTryCount = 3; // 最大同步尝试次数
    static const int MaxListSize = 10; // 单条消息允许包含最大记录数，超过此值需分多条消息发送
};

#endif //OSLA_NEW_TOOLS_H
