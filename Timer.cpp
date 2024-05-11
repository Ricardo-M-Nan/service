#include "Timer.h"
#include <time.h>
#include <chrono>
#include "Event.h"
#include "EventScheduler.h"
#include "Poller.h"
#include "Log.h"

Timer::Timer(Timer_Event* event, _Timestamp timestamp, _TimeInterval timeInterval, _TimerId timerId) :
    Time_Event(event),
    Time_stamp(timestamp),
    Time_Interval(timeInterval),
    Timer_Id(timerId) {
    if (timeInterval > 0) {
        Timer_Repeat = true;// 循环定时器
    }
    else {
        Timer_Repeat = false;//一次性定时器
    }
}
Timer::~Timer()
{

}
Timer::_Timestamp Timer::get_CurTime()// 获取系统从启动到目前的毫秒数
{
    long long now = std::chrono::steady_clock::now().time_since_epoch().count();
    return now / 1000000;
}

Timer::_Timestamp Timer::get_CurTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

}
bool Timer::Timer_handle_Event()
{
    if (!Time_Event) {
        return false;
    }
    return  Time_Event->handle_Timer_Event();
}
TimerManager* TimerManager::createNew(Event_Scheduler* scheduler) {

    if (!scheduler)
        return NULL;
    return new TimerManager(scheduler);
}
TimerManager::TimerManager(Event_Scheduler* scheduler) :
    TM_Poller(scheduler->poller()),
    TM_LastTimerId(0) {
    scheduler->setTimerManagerReadCallback(readCallback, this);
}

TimerManager::~TimerManager()
{

}
Timer::_TimerId TimerManager::addTimer(Timer_Event* event, Timer::_Timestamp timestamp,Timer::_TimeInterval timeInterval)
{
    ++TM_LastTimerId;
    Timer timer(event, timestamp, timeInterval, TM_LastTimerId);
    TM_Timers.insert(std::make_pair(TM_LastTimerId, timer));
    TM_Events.insert(std::make_pair(timestamp, timer));
    return TM_LastTimerId;
}
bool TimerManager::removeTimer(Timer::_TimerId timerId)
{
    std::map<Timer::_TimerId, Timer>::iterator it = TM_Timers.find(timerId);
    if (it != TM_Timers.end())
    {
        TM_Timers.erase(timerId);
        // TODO 还需要删除mEvents的事件
    }
    return true;
}
void TimerManager::readCallback(void* arg) {
    TimerManager* timerManager = (TimerManager*)arg;
    timerManager->TM_handleRead();
}
void TimerManager::TM_handleRead() {
    Timer::_Timestamp timestamp = Timer::get_CurTime();
    if (!TM_Timers.empty() && !TM_Events.empty()) {

        std::multimap<Timer::_Timestamp, Timer>::iterator it = TM_Events.begin();
        Timer timer = it->second;
        int expire = timer.Time_stamp - timestamp;
        if (timestamp > timer.Time_stamp || expire == 0) {

            bool timerEventIsStop = timer.Timer_handle_Event();
            TM_Events.erase(it);
            if (timer.Timer_Repeat) {
                if (timerEventIsStop) {
                    TM_Timers.erase(timer.Timer_Id);
                }
                else {
                    timer.Time_stamp = timestamp + timer.Time_Interval;
                    TM_Events.insert(std::make_pair(timer.Time_stamp, timer));
                }
            }
            else {
                TM_Timers.erase(timer.Timer_Id);
            }

        }
    }
}
