#ifndef _Socketsapi_H
#define _Socketsapi_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include<string>
#pragma warning( disable : 4996 )
namespace sockets
{
    int createTcpSock();//Ĭ�ϴ�����������tcp������
    int createUdpSock();//Ĭ�ϴ�����������udp������
    bool bind(int sockfd, std::string ip, uint16_t port);
    bool listen(int sockfd, int backlog);
    int accept(int sockfd);
    // ͨ������������д����
    int write(int sockfd, const void* buf, int size);// tcp д��
    int sendto(int sockfd, const void* buf, int len, const struct sockaddr* destAddr); // udp д��
    int setNonBlock(int sockfd);// ���÷�����ģʽ
    void setReuseAddr(int sockfd, int on);
    void setReusePort(int sockfd);
    void setNoDelay(int sockfd);
    void setKeepAlive(int sockfd);
    void setNoSigpipe(int sockfd);
    void setSendBufSize(int sockfd, int size);
    void setRecvBufSize(int sockfd, int size);
    std::string getPeerIp(int sockfd);
    int16_t getPeerPort(int sockfd);
    int getPeerAddr(int sockfd, struct sockaddr_in* addr);
    void close(int sockfd);
    bool connect(int sockfd, std::string ip, uint16_t port, int timeout);
    std::string getLocalIp();
}
#endif

