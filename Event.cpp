#include <iostream>
#include"Event.h"
#include"Log.h"
using namespace std;
Trigger_Event* Trigger_Event::createNew(void* arg)
{
	return new Trigger_Event(arg);
}
Trigger_Event* Trigger_Event::createNew()
{
	return new Trigger_Event(NULL);
}
Trigger_Event::Trigger_Event(void* arg) 
	: TriggerE_Arg(arg), TriggerE_CB(NULL) 
{
	LOGI("Trigger_Event()");
}
Trigger_Event::~Trigger_Event()
{
	LOGI("~TriggerEvent()");
}
void Trigger_Event::handle_Trigger_Event()
{
	if (TriggerE_CB)
	{
		TriggerE_CB(TriggerE_Arg);
	}
}


Timer_Event* Timer_Event::createNew(void* arg) 
{
	return new Timer_Event(arg);
}
Timer_Event* Timer_Event::createNew() {
	return new Timer_Event(NULL);
}
Timer_Event::Timer_Event(void* arg) 
	: Timer_Arg(arg),Timeout_CB(NULL),IsStop(false) 
{
	LOGI("TimerEvent()");
}
Timer_Event::~Timer_Event() 
{
	LOGI("~TimerEvent()");
}
bool Timer_Event::handle_Timer_Event()
{
	if (IsStop) 
	{
		return IsStop;
	}
	if (Timeout_CB) 
	{
		Timeout_CB(Timer_Arg);
	}
	return IsStop;
}
void Timer_Event::stop() 
{
	IsStop = true;
}


IO_Event* IO_Event::createNew(int fd, void* arg) {
	if (fd < 0)
	{
		return NULL;
	}
	return new IO_Event(fd, arg);
}
IO_Event* IO_Event::createNew(int fd) {
	if (fd < 0)
	{
		return NULL;
	}
	return new IO_Event(fd, NULL);
}
IO_Event::IO_Event(int fd, void* arg) :
	IOE_Fd(fd),IOE_Arg(arg),IOE_Event(EVENT_NONE),IO_R_Event(EVENT_NONE),IO_ReadCallback(NULL),IO_WriteCallback(NULL),IO_ErrorCallback(NULL) 
{
	LOGI("IOEvent() fd=%d", IOE_Fd);
}
IO_Event::~IO_Event() {
	LOGI("~IOEvent() fd=%d", IOE_Fd);
}
void IO_Event::handle_IO_Event()
{
	if (IO_ReadCallback && (IO_R_Event & EVENT_READ))
	{
		IO_ReadCallback(IOE_Arg);
	}

	if (IO_WriteCallback && (IO_R_Event & EVENT_WRITE))
	{
		IO_WriteCallback(IOE_Arg);
	}

	if (IO_ErrorCallback && (IO_R_Event & EVENT_ERROR))
	{
		IO_ErrorCallback(IOE_Arg);
	}
};