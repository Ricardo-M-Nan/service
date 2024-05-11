#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H
#include "UsageEnvironment.h"
#include "Event.h"
#include "Buffer.h"

class TcpConnection
{
public:
    typedef void (*DisConnectCallback)(void*, int);

    TcpConnection(UsageEnvironment* env, int clientFd);
    virtual ~TcpConnection();

    void setDisConnectCallback(DisConnectCallback cb, void* arg);

protected:
    void enableReadHandling();
    void enableWriteHandling();
    void enableErrorHandling();
    void disableReadeHandling();
    void disableWriteHandling();
    void disableErrorHandling();

    void handleRead();
    virtual void handleReadBytes();
    virtual void handleWrite();
    virtual void handleError();

    void handleDisConnect();

private:
    static void readCallback(void* arg);
    static void writeCallback(void* arg);
    static void errorCallback(void* arg);

protected:
    UsageEnvironment* mEnv;
    int mClientFd;
    IO_Event* mClientIOEvent;
    DisConnectCallback mDisConnectCallback;//��RtspServerʵ�������������ʵ��ʱ�����õĻص�����
    void* mArg;
    Buffer mInputBuffer;
    Buffer mOutBuffer;
    char mBuffer[2048];
};
#endif