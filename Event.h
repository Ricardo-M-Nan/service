#ifndef _EVENT_H
#define _EVENT_H
typedef void (*Event_FunP)(void*);
class Trigger_Event
{
public:
	static Trigger_Event* createNew(void* arg);
	static Trigger_Event* createNew();
	Trigger_Event(void* arg);
	~Trigger_Event();
	void setArg(void* arg)
	{ 
		TriggerE_Arg = arg;
	}
	void setTriggerCallback(Event_FunP cb)
	{ 
		TriggerE_CB = cb;
	}
	void handle_Trigger_Event();
private:
	void* TriggerE_Arg;
	Event_FunP TriggerE_CB;
};

class Timer_Event {
public:
	static Timer_Event* createNew(void* arg);
	static Timer_Event* createNew();

	Timer_Event(void* arg);
	~Timer_Event();

	void setArg(void* arg)
	{ 
		Timer_Arg = arg; 
	}
	void setTimeoutCallback(Event_FunP cb)
	{ 
		Timeout_CB = cb; 
	}
	bool handle_Timer_Event();

	void stop();

private:
	void* Timer_Arg;
	Event_FunP Timeout_CB;
	bool IsStop;
};

class IO_Event {
public:
    enum IO_EventType
    {
        EVENT_NONE = 0,
        EVENT_READ = 1,
        EVENT_WRITE = 2,
        EVENT_ERROR = 4,
    };

    static IO_Event* createNew(int fd, void* arg);
    static IO_Event* createNew(int fd);

    IO_Event(int fd, void* arg);
    ~IO_Event();

    int getFd() const
    { 
        return IOE_Fd;
    }
    int getEvent() const 
    { 
        return IOE_Event; 
    }
    void setREvent(int event) 
    { 
        IO_R_Event = event; 
    }
    void setArg(void* arg) 
    { 
        IOE_Arg = arg; 
    }

    void setReadCallback(Event_FunP cb)
    {
        IO_ReadCallback = cb;
    }
    void setWriteCallback(Event_FunP cb) 
    { 
        IO_WriteCallback = cb;
    };
    void setErrorCallback(Event_FunP cb) 
    { 
        IO_ErrorCallback = cb;
    };

    void enableReadHandling() 
    { 
        IOE_Event |= EVENT_READ;
    }
    void enableWriteHandling() 
    { 
        IOE_Event |= EVENT_WRITE;
    }
    void enableErrorHandling() 
    { 
        IOE_Event |= EVENT_ERROR; 
    }
    void disableReadeHandling() 
    {
        IOE_Event &= ~EVENT_READ; 
    }
    void disableWriteHandling() 
    { 
        IOE_Event &= ~EVENT_WRITE; 
    }
    void disableErrorHandling() 
    { 
        IOE_Event &= ~EVENT_ERROR; 
    }

    bool isNoneHandling() const 
    { 
        return IOE_Event == EVENT_NONE; 
    }
    bool isReadHandling() const 
    { 
        return (IOE_Event & EVENT_READ) != 0; 
    }
    bool isWriteHandling() const 
    { 
        return (IOE_Event & EVENT_WRITE) != 0; 
    }
    bool isErrorHandling() const 
    { 
        return (IOE_Event & EVENT_ERROR) != 0; 
    }

    void handle_IO_Event();

private:
    int IOE_Fd;
    void* IOE_Arg;
    int IOE_Event;
    int IO_R_Event;
    Event_FunP IO_ReadCallback;
    Event_FunP IO_WriteCallback;
    Event_FunP IO_ErrorCallback;
};
#endif

