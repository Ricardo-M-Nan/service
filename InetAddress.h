#ifndef _INETADDRESS_H
#define _INETADDRESS_H
#include <string>
#include <stdint.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
class Ipv4Address
{
public:
    Ipv4Address();
    Ipv4Address(std::string ip, uint16_t port);
    void setAddr(std::string ip, uint16_t port);
    std::string getIp();
    uint16_t getPort();
    struct sockaddr* getAddr();

private:
    std::string mIp;
    uint16_t mPort;
    struct sockaddr_in mAddr;
};

#endif
