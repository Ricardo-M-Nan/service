#ifndef _POLLER_H
#define _POLLER_H
#include <map>
#include "Event.h"
class Poller//ÂÖÑ¯Æ÷
{
public:
     ~Poller();
     virtual bool addIOEvent(IO_Event* event) = 0;
     virtual bool updateIOEvent(IO_Event* event) = 0;
     virtual bool removeIOEvent(IO_Event* event) = 0;
     virtual void handle_Poller_Event() = 0;
protected:
    Poller();
protected:
    typedef std::map<int, IO_Event*> IO_Event_Map;
    IO_Event_Map IOEMap;
};

#endif
