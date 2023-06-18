#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <functional>
#include <vector>
#include <cstring>
#include <poll.h>
#include <algorithm>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <thread> 
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>

#include "Watch.h"
#include "EventArgs.h"

namespace vivi
{

class EventLoop
{
    enum class EventState : uint8_t {
        NONE                = 0,
        FD_LIST_MODIFIED    = static_cast<uint8_t>( 0x01 << 0),
        QUEUE_READY         = static_cast<uint8_t>( 0x01 << 1),
        QUIT_EVENT_LOOP     = static_cast<uint8_t>( 0x01 << 2)
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
        ~EventLoop();

        void killEventLoop();

        bool addWatchToEventLoop(const Watch& watch);

        bool removeWatchFromEventLoop(const Watch& watch );

        bool runEventLoop(bool isThread = false);
        
        void subscribe(const std::string& signal, std::function<void (const std::shared_ptr<EventArgs>& eventArgs)> slot);

        void publish(const std::string& signal, const std::shared_ptr<EventArgs>& eventArgs = nullptr);            
        
    private:  
        void updatePool();

        void run();

        void watchNotify(int fd);

        void eventNotify();

        void notify(const EventLoop::EventState state);
        
        std::atomic<bool> m_exitEventLoop; 
        std::mutex m_mutex;

        struct pollfd *m_fds;
        size_t m_fdsCount;
        int m_eventFd ;

        //l'utilisateur peut lancer the eventLoop dans un thread (par default il n'est pas utilisé)
        std::thread m_eventLoopThread;

        //contient la list temporaire des watch a ajouté
        std::vector<Watch> m_watchToAdd;

        //contient la list temporaire des watch a supprimé
        std::vector<Watch> m_watchToDel;

        //indique qu'il faut mettre à jour le poll des evenement
        std::atomic<bool> m_updatePool;

        //contient la liste de tous les fd-callback en attente d'evenement
        std::vector<Watch> m_watchList;

        //contient la liste des subscribers en attente d'un publisher, std::pair<signal, callback>
        std::vector<std::pair<std::string, std::function<void (const std::shared_ptr<EventArgs>&)>>> m_subscriberList ;
    
        //contient la liste des callback a appelé avec les arguments par l'eventLoop 
        std::queue< EventLoop::Slot > m_queue; 

        //donne l'état des evenements
        uint8_t m_state;
};

}

#endif
