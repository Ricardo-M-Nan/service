#include"UsageEnvironment.h"
UsageEnvironment* UsageEnvironment::createNew(Event_Scheduler* scheduler, ThreadPool* threadPool)
{
    return new UsageEnvironment(scheduler, threadPool);
}

UsageEnvironment::UsageEnvironment(Event_Scheduler* scheduler, ThreadPool* threadPool) 
    :Env_Scheduler(scheduler), Env_ThreadPool(threadPool)
{

}

UsageEnvironment::~UsageEnvironment()
{

}

Event_Scheduler* UsageEnvironment::getscheduler()
{
    return Env_Scheduler;
}

ThreadPool* UsageEnvironment::getthreadPool()
{
    return Env_ThreadPool;
}