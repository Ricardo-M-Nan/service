#ifndef _SELECTPOLLER_H
#define _SELECTPOLLER_H
#include "Poller.h"
#include <vector>
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
class SelectPoller : public Poller
{
public:
    SelectPoller();
    ~SelectPoller();

    static SelectPoller* createNew();

     bool addIOEvent(IO_Event* event);
     bool updateIOEvent(IO_Event* event);
     bool removeIOEvent(IO_Event* event);
     void handle_Poller_Event();

private:
    fd_set ReadSet;
    fd_set WriteSet;
    fd_set ExceptionSet;
    int MaxNumSockets;
    std::vector<IO_Event*> SP_IOEvents;// 存储临时活跃的IO事件对象

};
#endif
