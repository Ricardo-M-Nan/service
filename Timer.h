#ifndef _Timer_H
#define _Timer_H
#include<stdint.h>
#include<map>
class Event_Scheduler;
class Poller;
class Timer_Event;
class IO_Event;
class Timer
{
public:
    typedef uint32_t _TimerId;
    typedef int64_t _Timestamp; //msʱ���
    typedef uint32_t _TimeInterval; //msʱ����

    ~Timer();

     static _Timestamp get_CurTime();// ��ȡ��ǰϵͳ���������ĺ�����
     static _Timestamp get_CurTimestamp();// ��ȡ���뼶ʱ�����13λ��

private:
    friend class TimerManager;
    Timer(Timer_Event* event, _Timestamp timestamp, _TimeInterval timeInterval, _TimerId timerId);

private:
    bool Timer_handle_Event();
private:
    Timer_Event* Time_Event;
    _Timestamp Time_stamp;
    _TimeInterval Time_Interval;
    _TimerId Timer_Id;

    bool Timer_Repeat;
};
class TimerManager
{
public:
    static TimerManager* createNew(Event_Scheduler* scheduler);

     TimerManager(Event_Scheduler* scheduler);
    ~TimerManager();

    Timer::_TimerId addTimer(Timer_Event* event, Timer::_Timestamp timestamp,Timer::_TimeInterval timeInterval);
    bool removeTimer(Timer::_TimerId timerId);

private:
    static void readCallback(void* arg);
    void TM_handleRead();
private:
    Poller* TM_Poller;
    std::map<Timer::_TimerId, Timer> TM_Timers;
    std::multimap<Timer::_Timestamp, Timer> TM_Events;
    uint32_t TM_LastTimerId;
};
#endif