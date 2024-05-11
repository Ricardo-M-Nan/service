#ifndef _EVENTSCHEDULER_H
#define _EVENTSCHEDULER_H
#include<queue>
#include<vector>
#include <mutex>
#include <stdint.h>
#include "Timer.h"
#include "Event.h"
class Poller;
class Event_Scheduler
{
public:
	static Event_Scheduler* createNew();
	Event_Scheduler();
	virtual ~Event_Scheduler();
    bool addTriggerEvent(Trigger_Event* event);
    Timer::_TimerId addTimedEventRunAfater(Timer_Event* event, Timer::_TimeInterval delay);
    Timer::_TimerId addTimedEventRunAt(Timer_Event* event, Timer::_Timestamp when);
    Timer::_TimerId addTimedEventRunEvery(Timer_Event* event, Timer::_TimeInterval interval);
    bool removeTimedEvent(Timer::_TimerId timerId);
    bool addIOEvent(IO_Event* event);
    bool updateIOEvent(IO_Event* event);
    bool removeIOEvent(IO_Event* event);

    void loop();
    Poller* poller();
    void setTimerManagerReadCallback(Event_FunP cb, void* arg);

private:
    void handleTriggerEvents();
private:
    bool ES_Quit;
    Poller* ES_Poller;
    TimerManager* ES_TimerManager;
    std::vector<Trigger_Event*> ES_TriggerEvents;

    std::mutex mMtx;

    // 定时器回调start
    Event_FunP ES_TimerManagerReadCallback;
    void* ES_TimerManagerArg;
    // 定时器回调end

};
#endif
