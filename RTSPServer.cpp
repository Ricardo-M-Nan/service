#include "RtspServer.h"
#include "RtspConnection.h"
#include "Log.h"

RtspServer* RtspServer::createNew(UsageEnvironment* env, MediaSessionManager* sessMgr, Ipv4Address& addr) {

    return new RtspServer(env, sessMgr, addr);
}
RtspServer::RtspServer(UsageEnvironment* env, MediaSessionManager* sessMgr, Ipv4Address& addr) :
    mSessMgr(sessMgr),
    mEnv(env),
    mAddr(addr),
    mListen(false)
{

    mFd = sockets::createTcpSock();
    sockets::setReuseAddr(mFd, 1);//��ַ����
    if (!sockets::bind(mFd, addr.getIp(), mAddr.getPort())) {
        return;
    }

    LOGI("rtsp://%s:%d fd=%d", addr.getIp().data(), addr.getPort(), mFd);

    mAcceptIOEvent = IO_Event::createNew(mFd, this);//����i0�¼�
    mAcceptIOEvent->setReadCallback(readCallback);//���ûص���socket�ɶ� ����ָ��
    mAcceptIOEvent->enableReadHandling();//����ص�����

    mCloseTriggerEvent = Trigger_Event::createNew(this);
    mCloseTriggerEvent->setTriggerCallback(cbCloseConnect);//���ûص��Ĺر����� ����ָ��

}

RtspServer::~RtspServer()
{
    if (mListen)
        mEnv->getscheduler()->removeIOEvent(mAcceptIOEvent);

    delete mAcceptIOEvent;
    delete mCloseTriggerEvent;

    sockets::close(mFd);
}



void RtspServer::start() {
    LOGI("");
    mListen = true;
    sockets::listen(mFd, 60);
    mEnv->getscheduler()->addIOEvent(mAcceptIOEvent);
}

void RtspServer::readCallback(void* arg) {
    RtspServer* rtspServer = (RtspServer*)arg;
    rtspServer->handleRead();

}

void RtspServer::handleRead() {
    int clientFd = sockets::accept(mFd);
    if (clientFd < 0)
    {
        LOGE("handleRead error,clientFd=%d", clientFd);
        return;
    }
    RtspConnection* conn = RtspConnection::createNew(this, clientFd);
    conn->setDisConnectCallback(RtspServer::cbDisConnect, this);
    mConnMap.insert(std::make_pair(clientFd, conn));

}
void RtspServer::cbDisConnect(void* arg, int clientFd) {
    RtspServer* server = (RtspServer*)arg;

    server->handleDisConnect(clientFd);
}

void RtspServer::handleDisConnect(int clientFd) {

    std::lock_guard <std::mutex> lck(mMtx);
    mDisConnList.push_back(clientFd);

    mEnv->getscheduler()->addTriggerEvent(mCloseTriggerEvent);

}

void RtspServer::cbCloseConnect(void* arg) {
    RtspServer* server = (RtspServer*)arg;
    server->handleCloseConnect();
}
void RtspServer::handleCloseConnect() {

    std::lock_guard <std::mutex> lck(mMtx);

    for (std::vector<int>::iterator it = mDisConnList.begin(); it != mDisConnList.end(); ++it) {

        int clientFd = *it;
        std::map<int, RtspConnection*>::iterator _it = mConnMap.find(clientFd);
        assert(_it != mConnMap.end());
        delete _it->second;
        mConnMap.erase(clientFd);
    }

    mDisConnList.clear();
}