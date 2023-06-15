#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <functional>
#include <vector>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <unistd.h>
#include <map>
#include <unordered_map>
#include <sys/eventfd.h>
#include <thread> 
#include <queue>
#include <memory>

#include "Watch.h"
#include "EventArgs.h"

namespace vivi
{

class EventLoop
{
    enum class EventState : uint64_t { //TODO unit8_t ??
        NONE                = 0,
        FD_LIST_MODIFIED    = static_cast<uint64_t>( 0x01 << 0),
        QUEUE_READY         = static_cast<uint64_t>( 0x01 << 1),
        QUIT_EVENT_LOOP     = static_cast<uint64_t>( 0x01 << 2)
    };

    struct Slot{                
        Slot(const std::function<void (const std::shared_ptr<EventArgs>& eventArgs)>& callback, const std::shared_ptr<EventArgs>& eventArgs):
            m_callback(callback),
            m_eventArgs(eventArgs)
        {}
        const std::function<void (const std::shared_ptr<EventArgs>& eventArgs)> m_callback;
        const std::shared_ptr<EventArgs> m_eventArgs;        
    };

    public:
        EventLoop();

        bool addWatchToEventLoop( const Watch & watch);

        bool removeWatchFromEventLoop( const Watch & watch );

        bool runEventLoop(bool isThread = false);

        //TODO strange name, using rather than subscrib and publish ?
        void subscribe(const std::string& signal, std::function<void (const std::shared_ptr<EventArgs>& eventArgs)> slot);

        void publish(const std::string& signal, const std::shared_ptr<EventArgs>& eventArgs = nullptr);            

        
    private:
        void run();

        void watchNotify(int fd);

        void eventNotify();

        void notify(const EventLoop::EventState state);
        
        bool m_exitEventLoop; 

        struct pollfd *m_fds; //TODO use epoll : more efectively
        size_t m_fdsCount;
        int m_eventFd ;

        //contient la liste de tous les fd-callback ou on attend un evenement
        std::vector<Watch> m_watchList;

        //contient la liste des subscribers en attente d'un publisher, std::pair<signal, callback>
        std::vector<std::pair<std::string, std::function<void (const std::shared_ptr<EventArgs>&)>>> m_subscriberList ;
    
        //contient la liste des callback a appelé avec les arguments par l'eventLoop 
        std::queue< EventLoop::Slot > m_queue; 

        //donne l'état des evenements
        uint64_t m_state;
};

}

#endif