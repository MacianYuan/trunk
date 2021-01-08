//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_CLIENTCONTEXT_H
#define OSLA_NEW_CLIENTCONTEXT_H

#include <object/BaseObject.h>
#include "sqlworker/sqlworker.h"

class SslService;
class SqlWorker;

class ClientContext : public BaseObject {
public:
    ClientContext(std::string name = "ClientContext");

    int getInstrumentId() const;

    void setInstrumentId(int instrumentId);

    int getClientState() const;

    void setClientState(int clientState);

    bool isNetworkStable() const;

    void setNetworkStable(bool networkStable);

    //SqlWorker *getSqlWorker() const;

    //void setSqlWorker(SqlWorker *sqlWorker);

    SslService *getSslService() const;

    void setSslService(SslService *sslService);

    bool getRestartEnabled() const;

    void setRestartEnabled(bool restartEnabled);

    bool isConnected() const;

    void setConnected(bool connected);

private:
    int instrumentId{-1};            /// 设备ID
    int clientState{0};              /// 设备状态
    bool networkStable{false};       /// 网络状态
    //SqlWorker *sqlWorker{nullptr};   /// 数据库连接
    SslService *sslService{nullptr}; /// 网络
    bool connected{false};           /// 网络是否连接
    bool restartEnabled{true};       /// 是否可以重启
};

#endif //OSLA_NEW_CLIENTCONTEXT_H
