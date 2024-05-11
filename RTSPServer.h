#ifndef _RTSPSERVER_H
#define _RTSPSERVER_H
#include <mutex>
#include "UsageEnvironment.h"
#include "Event.h"
#include "MediaSession.h"
#include "InetAddress.h"
class MediaSessionManager;
class RtspConnection;
class RtspServer {

public:
    static RtspServer* createNew(UsageEnvironment* env, MediaSessionManager* sessMgr, Ipv4Address& addr);

    RtspServer(UsageEnvironment* env, MediaSessionManager* sessMgr, Ipv4Address& addr);
    ~RtspServer();

public:
    MediaSessionManager* mSessMgr;
    void start();
    UsageEnvironment* env() const {
        return mEnv;
    }
private:
    static void readCallback(void*);
    void handleRead();
    static void cbDisConnect(void* arg, int clientFd);
    void handleDisConnect(int clientFd);
    static void cbCloseConnect(void* arg);
    void handleCloseConnect();

private:

    UsageEnvironment* mEnv;
    int  mFd;
    Ipv4Address mAddr;
    bool mListen;
    IO_Event* mAcceptIOEvent;
    std::mutex mMtx;

    std::map<int, RtspConnection*> mConnMap; // <clientFd,conn> 维护所有被创建的连接
    std::vector<int> mDisConnList;//所有被取消的连接 clientFd
    Trigger_Event* mCloseTriggerEvent;// 关闭连接的触发事件

};
#endif
