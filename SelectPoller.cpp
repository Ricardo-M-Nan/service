#include "SelectPoller.h"
#include "Log.h"

SelectPoller::SelectPoller()
{
    FD_ZERO(&ReadSet);
    FD_ZERO(&WriteSet);
    FD_ZERO(&ExceptionSet);
}
SelectPoller::~SelectPoller()
{

}
SelectPoller* SelectPoller::createNew()
{
    return new SelectPoller();
}

bool SelectPoller::addIOEvent(IO_Event* event)
{
    return updateIOEvent(event);
}
bool SelectPoller::updateIOEvent(IO_Event* event)
{
    int fd = event->getFd();
    if (fd < 0)
    {
        LOGE("fd=%d", fd);

        return false;
    }

    FD_CLR(fd, &ReadSet);
    FD_CLR(fd, &WriteSet);
    FD_CLR(fd, &ExceptionSet);

    IO_Event_Map::iterator it = IOEMap.find(fd);
    if (it != IOEMap.end()) //先前已经添加则修改
    {
        if (event->isReadHandling())
            FD_SET(fd, &ReadSet);
        if (event->isWriteHandling())
            FD_SET(fd, &WriteSet);
        if (event->isErrorHandling())
            FD_SET(fd, &ExceptionSet);
    }
    else //先前未添加则添加IO事件
    {
        if (event->isReadHandling())
            FD_SET(fd, &ReadSet);
        if (event->isWriteHandling())
            FD_SET(fd, &WriteSet);
        if (event->isErrorHandling())
            FD_SET(fd, &ExceptionSet);

        IOEMap.insert(std::make_pair(fd, event));
    }

    if (IOEMap.empty())
        MaxNumSockets = 0;
    else
        MaxNumSockets = IOEMap.rbegin()->first + 1; //更新最大文件描述符+1（map会自动排序）

    return true;
}

bool SelectPoller::removeIOEvent(IO_Event* event)
{
    int fd = event->getFd();
    if (fd < 0)
        return false;

    FD_CLR(fd, &ReadSet);
    FD_CLR(fd, &WriteSet);
    FD_CLR(fd, &ExceptionSet);

    IO_Event_Map::iterator it = IOEMap.find(fd);
    if (it != IOEMap.end())
        IOEMap.erase(it);

    if (IOEMap.empty())
        MaxNumSockets = 0;
    else
        MaxNumSockets = IOEMap.rbegin()->first + 1;
    return true;
}
void SelectPoller::handle_Poller_Event() {

    fd_set readSet = ReadSet;
    fd_set writeSet = WriteSet;
    fd_set exceptionSet = ExceptionSet;
    struct timeval timeout;
    int ret;
    int rEvent;

    timeout.tv_sec = 1000;// 秒
    timeout.tv_usec = 0;//微秒

    ret = select(MaxNumSockets, &readSet, &writeSet, &exceptionSet, &timeout);


    if (ret < 0) {
        return;
    }
    else {
        LOGI("检测到活跃的描述符 ret=%d", ret);
    }

    for (IO_Event_Map::iterator it = IOEMap.begin(); it != IOEMap.end(); ++it)
    {
        rEvent = 0;
        if (FD_ISSET(it->first, &readSet))
            rEvent |= IO_Event::EVENT_READ;

        if (FD_ISSET(it->first, &writeSet))
            rEvent |= IO_Event::EVENT_WRITE;

        if (FD_ISSET(it->first, &exceptionSet))
            rEvent |= IO_Event::EVENT_ERROR;

        if (rEvent != 0)
        {
            it->second->setREvent(rEvent);
            SP_IOEvents.push_back(it->second);
        }
    }

    for (auto& ioEvent : SP_IOEvents) {
        ioEvent->handle_IO_Event();
    }

    SP_IOEvents.clear();

}