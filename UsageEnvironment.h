#ifndef _USAGEENVIRONMENT_H
#define _USAGEENVIRONMENT_H
#include"EventScheduler.h"
#include"ThreadPool.h"
class UsageEnvironment
{
public:
    static UsageEnvironment* createNew(Event_Scheduler* scheduler, ThreadPool* threadPool);

    UsageEnvironment(Event_Scheduler* scheduler, ThreadPool* threadPool);
    ~UsageEnvironment();

    Event_Scheduler* getscheduler();
    ThreadPool* getthreadPool();

private:
    Event_Scheduler* Env_Scheduler;
    ThreadPool* Env_ThreadPool;
};
#endif
