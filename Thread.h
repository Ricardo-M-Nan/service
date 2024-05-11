#ifndef _THREAD_H
#define _THREAD_H
#include<thread>
class Thread
{
public:
    virtual ~Thread();

    bool start(void* arg);
    bool detach();
    bool join();

protected:
    Thread();

    virtual void run(void* arg) = 0;

private:
    static void* threadRun(void*);

private:
    void* Thread_Arg;
    bool IsStart;
    bool IsDetach;
    std::thread ThreadId;
};
#endif

