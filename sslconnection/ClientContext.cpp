//
// Created by 何继胜 on 2017/4/12.
//

#include "sslconnection/ClientContext.h"

ClientContext::ClientContext(std::string name) : BaseObject(name) {

}

int ClientContext::getInstrumentId() const {
    return instrumentId;
}

void ClientContext::setInstrumentId(int instrumentId) {
    ClientContext::instrumentId = instrumentId;
}

int ClientContext::getClientState() const {
    return clientState;
}

void ClientContext::setClientState(int clientState) {
    ClientContext::clientState = clientState;
}

bool ClientContext::isNetworkStable() const {
    return networkStable;
}

void ClientContext::setNetworkStable(bool networkStable) {
    ClientContext::networkStable = networkStable;
}

//SqlWorker *ClientContext::getSqlWorker() const {
//    return sqlWorker;
//}
//
//void ClientContext::setSqlWorker(SqlWorker *sqlWorker) {
//    ClientContext::sqlWorker = sqlWorker;
//}

SslService *ClientContext::getSslService() const {
    return sslService;
}

void ClientContext::setSslService(SslService *sslService) {
    ClientContext::sslService = sslService;
}

bool ClientContext::getRestartEnabled() const {
    return restartEnabled;
}

void ClientContext::setRestartEnabled(bool restartEnabled) {
    ClientContext::restartEnabled = restartEnabled;
}

bool ClientContext::isConnected() const {
    return connected;
}

void ClientContext::setConnected(bool connected) {
    ClientContext::connected = connected;
}
