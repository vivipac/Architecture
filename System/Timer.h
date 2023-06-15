#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <functional>
#include <sys/timerfd.h>

#include "EventLoop.h"

namespace vivi
{

class Timer
{
    public:
    Timer(int interval, EventLoop & eventLoop);

    Timer( const Timer&) = delete;

    Timer & operator=(const Timer&)= delete;

    ~Timer();

    void addWatchToEventLoop(const std::function<void (void)>& inCallback );

    template<class T_Obj>
    inline void addWatchToEventLoop(T_Obj* obj, void (T_Obj::*callback)(void)){
        addWatchToEventLoop( std::bind( callback, obj));
    }

    void start(int delay_sec = 0);    

    void stop();

    bool isTimerActivated() const;    

    private:  
    void removeWatchFromEventLoop();  
    
    bool m_isTimerActivated;
    int m_fd;
    int m_interval;
    EventLoop & m_eventLoop;
};

}

#endif