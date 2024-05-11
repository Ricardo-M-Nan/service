#include "TcpConnection.h"
#include "Socketsapi.h"
#include "Log.h"


TcpConnection::TcpConnection(UsageEnvironment* env, int clientFd) :
    mEnv(env),
    mClientFd(clientFd)
{
    mClientIOEvent = IO_Event::createNew(clientFd, this);
    mClientIOEvent->setReadCallback(readCallback);
    mClientIOEvent->setWriteCallback(writeCallback);
    mClientIOEvent->setErrorCallback(errorCallback);
    mClientIOEvent->enableReadHandling(); //Ĭ��ֻ������

    //    mClientIOEvent->enableWriteHandling();
    //    mClientIOEvent->enableErrorHandling();

    mEnv->getscheduler()->addIOEvent(mClientIOEvent);
}

TcpConnection::~TcpConnection()
{
    mEnv->getscheduler()->removeIOEvent(mClientIOEvent);
    delete mClientIOEvent;
    //    Delete::release(mClientIOEvent);

    sockets::close(mClientFd);
}

void TcpConnection::setDisConnectCallback(DisConnectCallback cb, void* arg)
{
    mDisConnectCallback = cb;
    mArg = arg;
}

void TcpConnection::enableReadHandling()
{
    if (mClientIOEvent->isReadHandling())
        return;

    mClientIOEvent->enableReadHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::enableWriteHandling()
{
    if (mClientIOEvent->isWriteHandling())
        return;

    mClientIOEvent->enableWriteHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::enableErrorHandling()
{
    if (mClientIOEvent->isErrorHandling())
        return;

    mClientIOEvent->enableErrorHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableReadeHandling()
{
    if (!mClientIOEvent->isReadHandling())
        return;

    mClientIOEvent->disableReadeHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableWriteHandling()
{
    if (!mClientIOEvent->isWriteHandling())
        return;

    mClientIOEvent->disableWriteHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableErrorHandling()
{
    if (!mClientIOEvent->isErrorHandling())
        return;

    mClientIOEvent->disableErrorHandling();
    mEnv->getscheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::handleRead() {

    //LOGI("");
    int ret = mInputBuffer.read(mClientFd);

    if (ret <= 0)
    {
        LOGE("read error,fd=%d,ret=%d", mClientFd, ret);
        handleDisConnect();
        return;
    }

    handleReadBytes();// ����RtspConnecton�����ʵ�ֺ��� 
}

void TcpConnection::handleReadBytes() {
    LOGI("");

    mInputBuffer.retrieveAll();
}
void TcpConnection::handleDisConnect()
{
    if (mDisConnectCallback) {
        mDisConnectCallback(mArg, mClientFd);
    }
}
void TcpConnection::handleWrite()
{
    LOGI("");
    mOutBuffer.retrieveAll();

}

void TcpConnection::handleError()
{
    LOGI("");
}

void TcpConnection::readCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleRead();
}

void TcpConnection::writeCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleWrite();
}

void TcpConnection::errorCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleError();
}
