#include"Thread.h"
Thread::Thread() 
    : Thread_Arg(nullptr),IsStart(false),IsDetach(false)
{

}
Thread::~Thread()
{
    if (IsStart == true && IsDetach == false)
        detach();
}

bool Thread::start(void* arg)
{
    Thread_Arg = arg;
    ThreadId = std::thread(&Thread::threadRun, this);
    IsStart = true;
    return true;
}

bool Thread::detach()
{
    if (IsStart != true)
        return false;
    if (IsDetach == true)
        return true;
    ThreadId.detach();
    IsDetach = true;
    return true;
}

bool Thread::join()
{
    if (IsStart != true || IsDetach == true)
        return false;
    ThreadId.join();
    return true;
}

void* Thread::threadRun(void* arg)
{
    Thread* thread = (Thread*)arg;
    thread->run(thread->Thread_Arg);
    return NULL;
}