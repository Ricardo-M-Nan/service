#include "Socketsapi.h"
#include <fcntl.h>
#include <sys/types.h>  
#include"Log.h"

int sockets::createTcpSock()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    unsigned long ul = 1;
    int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);//FIONBIO设置为非阻塞

    if (ret == SOCKET_ERROR) {
        LOGE("设置非阻塞失败");
    }
    return sockfd;
}

int sockets::createUdpSock()
{
    int sockfd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    unsigned long ul = 1;
    int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);

    if (ret == SOCKET_ERROR) {
        LOGE("设置非阻塞失败");
    }
    return sockfd;

}

bool sockets::bind(int sockfd, std::string ip, uint16_t port)
{
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        LOGE("::bind error,fd=%d,ip=%s,port=%d", sockfd, ip.c_str(), port);
        return false;
    }
    return true;
}

bool sockets::listen(int sockfd, int backlog)
{
    if (::listen(sockfd, backlog) < 0) {
        LOGE("::listen error,fd=%d,backlog=%d", sockfd, backlog);
        return false;
    }
    return true;
}

int sockets::accept(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int connfd = ::accept(sockfd, (struct sockaddr*)&addr, &addrlen);
    return connfd;
}

int sockets::write(int sockfd, const void* buf, int size)
{
    return ::send(sockfd, (char*)buf, size, 0);
}

int sockets::sendto(int sockfd, const void* buf, int len,const struct sockaddr* destAddr)
{
    socklen_t addrLen = sizeof(struct sockaddr);
    return ::sendto(sockfd, (char*)buf, len, 0, destAddr, addrLen);
}

int sockets::setNonBlock(int sockfd)
{
    unsigned long ul = 1;
    int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);//设置非阻塞

    if (ret == SOCKET_ERROR) {
        return -1;
    }
    else {
        return 0;
    }

}

void sockets::setReuseAddr(int sockfd, int on)//设置端口复用
{
    int optval = on ? 1 : 0;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
}

void sockets::setNoDelay(int sockfd)
{
#ifdef TCP_NODELAY
    int on = 1;
    int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on));
#endif
}

void sockets::setKeepAlive(int sockfd)
{
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on));
}


void sockets::setNoSigpipe(int sockfd)
{
#ifdef SO_NOSIGPIPE
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (char*)&on, sizeof(on));
#endif
}

void sockets::setSendBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));
}

void sockets::setRecvBufSize(int sockfd, int size)
{
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
}

std::string sockets::getPeerIp(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr*)&addr, &addrlen) == 0)
    {
        return inet_ntoa(addr.sin_addr);
    }

    return "0.0.0.0";
}

int16_t sockets::getPeerPort(int sockfd)
{
    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if (getpeername(sockfd, (struct sockaddr*)&addr, &addrlen) == 0)
    {
        return ntohs(addr.sin_port);
    }

    return 0;
}

int sockets::getPeerAddr(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    return getpeername(sockfd, (struct sockaddr*)addr, &addrlen);
}

void sockets::close(int sockfd)
{
    int ret = ::closesocket(sockfd);
}

void sockets::setReusePort(int sockfd)
{
#ifdef SO_REUSEPORT
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on));
#endif
}

bool sockets::connect(int sockfd, std::string ip, uint16_t port, int timeout)
{
    bool isConnected = true;
    if (timeout > 0)
    {
        sockets::setNonBlock(sockfd);
    }

    struct sockaddr_in addr = { 0 };
    socklen_t addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (::connect(sockfd, (struct sockaddr*)&addr, addrlen) < 0)
    {
        if (timeout > 0)
        {
            isConnected = false;
            fd_set fdWrite;
            FD_ZERO(&fdWrite);
            FD_SET(sockfd, &fdWrite);
            struct timeval tv = { timeout / 1000, timeout % 1000 * 1000 };
            select(sockfd + 1, NULL, &fdWrite, NULL, &tv);
            if (FD_ISSET(sockfd, &fdWrite))
            {
                isConnected = true;
            }
        }
        else
        {
            isConnected = false;
        }
    }
    return isConnected;
}

std::string sockets::getLocalIp()
{
    return "0.0.0.0";
}