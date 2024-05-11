#include "EventScheduler.h"
#include "SelectPoller.h"
#include "Log.h"
Event_Scheduler* Event_Scheduler::createNew()
{
    return new Event_Scheduler();
}
Event_Scheduler::Event_Scheduler() : ES_Quit(false)
{
    WSADATA wdSockMsg;
    int s = WSAStartup(MAKEWORD(2, 2), &wdSockMsg);

    if (0 != s)
    {
        switch (s)
        {
        case WSASYSNOTREADY: {
            LOGE("重启电脑，或者检查网络库");
            break;
        }
        case WSAVERNOTSUPPORTED: {
            LOGE("请更新网络库");
            break;
        }
        case WSAEINPROGRESS: {
            LOGE("请重新启动");
            break;
        }
        case WSAEPROCLIM: {
            LOGE("请关闭不必要的软件，以确保有足够的网络资源"); break;
        }
        }
    }

    if (2 != HIBYTE(wdSockMsg.wVersion) || 2 != LOBYTE(wdSockMsg.wVersion))
    {
        LOGE("网络库版本错误");
        return;
    }
    ES_Poller = SelectPoller::createNew();
    ES_TimerManager = TimerManager::createNew(this);//定时器由子线程托管

}
Event_Scheduler::~Event_Scheduler()
{
    delete ES_TimerManager;
    delete ES_Poller;
    WSACleanup();
}
bool Event_Scheduler::addTriggerEvent(Trigger_Event* event)
{
    ES_TriggerEvents.push_back(event);
    return true;
}

Timer::_TimerId Event_Scheduler::addTimedEventRunAfater(Timer_Event* event, Timer::_TimeInterval delay)
{
    Timer::_Timestamp timestamp = Timer::get_CurTime();
    timestamp += delay;

    return ES_TimerManager->addTimer(event, timestamp, 0);
}

Timer::_TimerId Event_Scheduler::addTimedEventRunAt(Timer_Event* event, Timer::_Timestamp when)
{
    return ES_TimerManager->addTimer(event, when, 0);
}

Timer::_TimerId Event_Scheduler::addTimedEventRunEvery(Timer_Event* event, Timer::_TimeInterval interval)
{
    Timer::_Timestamp timestamp = Timer::get_CurTime();
    timestamp += interval;

    return ES_TimerManager->addTimer(event, timestamp, interval);
}

bool Event_Scheduler::removeTimedEvent(Timer::_TimerId timerId)
{
    return ES_TimerManager->removeTimer(timerId);
}

bool Event_Scheduler::addIOEvent(IO_Event* event)
{
    return ES_Poller->addIOEvent(event);
}

bool Event_Scheduler::updateIOEvent(IO_Event* event)
{
    return ES_Poller->updateIOEvent(event);
}

bool Event_Scheduler::removeIOEvent(IO_Event* event)
{
    return ES_Poller->removeIOEvent(event);
}

void Event_Scheduler::loop() {

    std::thread([](Event_Scheduler* sch) {
        while (!sch->ES_Quit) {
            if (sch->ES_TimerManagerReadCallback) {
                sch->ES_TimerManagerReadCallback(sch->ES_TimerManagerArg);
            }
        }
        }, this).detach();
        while (!ES_Quit) {
            handleTriggerEvents();
            ES_Poller->handle_Poller_Event();
        }
}

void Event_Scheduler::handleTriggerEvents()
{
    if (!ES_TriggerEvents.empty())
    {
        for (std::vector<Trigger_Event*>::iterator it = ES_TriggerEvents.begin();it != ES_TriggerEvents.end(); ++it)
        {
            (*it)->handle_Trigger_Event();
        }

        ES_TriggerEvents.clear();
    }
}
Poller* Event_Scheduler::poller() {
    return ES_Poller;
}
void Event_Scheduler::setTimerManagerReadCallback(Event_FunP cb, void* arg)
{
    ES_TimerManagerReadCallback = cb;
    ES_TimerManagerArg = arg;
}