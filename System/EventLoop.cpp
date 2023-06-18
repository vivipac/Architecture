#include "EventLoop.h"
#include <memory>
#include <functional>

#include <string.h>
#include <errno.h>

namespace vivi
{

EventLoop::EventLoop(): 
    m_fds(nullptr),
    m_fdsCount(0),
    m_eventFd(-1),
    m_exitEventLoop( false ),
    m_state(static_cast<uint8_t>(EventLoop::EventState::NONE))
{    
    //init events notifier 
    if ( (m_eventFd = ::eventfd(static_cast<unsigned long>(0), 0)) < 0) 
    {
        std::cerr << "eventfd syscall error : " << std::strerror(errno) << "\n";
        ::exit(EXIT_FAILURE);
    }
    
    Watch w (m_eventFd, [this](){
        eventNotify();
    }); 
    m_watchList.push_back(w);

    m_fds = static_cast<struct pollfd*> ( ::malloc( sizeof( struct pollfd) ) );
    m_fds[m_fdsCount].fd = m_eventFd;
    m_fds[m_fdsCount].events = POLLIN | POLLHUP | POLLERR ;

    m_fdsCount++;    
}

EventLoop::~EventLoop()
{
    if( m_eventLoopThread.joinable())
    {
        m_exitEventLoop = true;
        m_eventLoopThread.join();
    }
}

void EventLoop::killEventLoop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    notify( vivi::EventLoop::EventState::QUIT_EVENT_LOOP);
}

bool EventLoop::removeWatchFromEventLoop( const ::vivi::Watch& watch )
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_watchToDel.push_back(watch);
    
    notify(EventLoop::EventState::FD_LIST_MODIFIED);

    return true; 
}

bool EventLoop::addWatchToEventLoop( const ::vivi::Watch& watch)
{   
    std::lock_guard<std::mutex> lock(m_mutex);
    m_watchToAdd.push_back(watch);
    
    notify(EventLoop::EventState::FD_LIST_MODIFIED);    
    return true; 
}

void EventLoop::updatePool() 
{    
    std::lock_guard<std::mutex> lock(m_mutex);     

    struct pollfd* tmp = m_fds;

    //add watch
    if(!m_watchToAdd.empty())
    {
        tmp = static_cast<struct pollfd*> ( ::malloc( sizeof( struct pollfd) * (m_fdsCount + m_watchToAdd.size()) ) );

        ::memcpy(tmp, m_fds, m_fdsCount * sizeof( struct pollfd));
        
        for(const auto& watch : m_watchToAdd)
        {
            tmp[m_fdsCount].fd = watch.fd();
            tmp[m_fdsCount].events = POLLIN | POLLHUP | POLLERR ;

            m_watchList.push_back( watch );

            m_fdsCount++;
        }
    }    

    //delete Watch
    if(!m_watchToDel.empty())    
    {
        struct pollfd* tmp2 = static_cast<struct pollfd*> ( ::malloc( sizeof( struct pollfd) * (m_fdsCount - m_watchToDel.size()) ) );    
        
        for (size_t i = 0; i < m_fdsCount; i++)
        {
            bool adding = true;
            for(const auto& watch : m_watchToDel)
            {
                if( tmp[i].fd == watch.fd())
                {                
                    adding = false;
                    break;
                }                
            }
            if(adding)
            {
                tmp2[i].fd = tmp[i].fd;
                tmp2[i].events = POLLIN | POLLHUP | POLLERR ;
            }        
        }  

        for(auto it = m_watchList.begin(); it != m_watchList.end(); ++it)
        {
            for(const auto & watch: m_watchToDel)
            {
                if( watch.fd() == it->fd())
                {
                    it = m_watchList.erase(it);
                }
            }
        }

        tmp = tmp2;
    }    
                
    ::free(m_fds);
    m_fds = tmp;
}

void EventLoop::watchNotify(int fd)
{
    for( auto & watch : m_watchList)
    {
        if( watch.fd() == fd)
        {
            watch.callback();   
            if( !watch.needData() )    
            {
                // discard the buffer
                char bufferDump[1024];
                
                int readData;
                do{
                    readData = ::read(watch.fd(), bufferDump, 1024);
                }while( readData >= 1024);
                
            }
        }                           
    }    
}

void EventLoop::eventNotify() 
{  
    std::queue<EventLoop::Slot> slots; 
    {    
        std::lock_guard<std::mutex> lock(m_mutex);

        uint64_t eventFdRequest = 1;
        ::read(m_eventFd, &eventFdRequest, sizeof(uint64_t));        
        
        if( m_state & static_cast<uint8_t>(EventLoop::EventState::QUEUE_READY))
        {               
            m_state &= ~static_cast<uint8_t>(EventLoop::EventState::QUEUE_READY);   

            slots.swap(m_queue);        
        }
        if(m_state & static_cast<uint8_t>(EventLoop::EventState::FD_LIST_MODIFIED))
        {
            m_state &= ~static_cast<uint8_t>(EventLoop::EventState::FD_LIST_MODIFIED);

            m_updatePool = true;                
        }

        if(m_state & static_cast<uint8_t>(EventLoop::EventState::QUIT_EVENT_LOOP))
        {
            m_state &= ~static_cast<uint8_t>(EventLoop::EventState::QUIT_EVENT_LOOP);

            m_exitEventLoop = true;
        } 
    }

    while( slots.size() > 0 )
    {                    
        auto slot = slots.front(); 
        slots.pop();                                                   
        slot.m_callback(slot.m_eventArgs);
    }      
    
}

void EventLoop::run(){ 

    while(!m_exitEventLoop)
    {        
        int nEvents = ::poll( m_fds, m_fdsCount, -1);
        
        for (size_t i = 0; (i < m_fdsCount) || (nEvents > 0) ; i++)
        {
            if( m_fds[i].revents == POLLIN)
            {
                nEvents--;                
                                   
                watchNotify(m_fds[i].fd);                
            }
        }

        if(m_updatePool)
        {
            m_updatePool = false;
            updatePool();
        } 
    }
}

bool EventLoop::runEventLoop(bool isThread)
{
    if( isThread )
    {        
        m_eventLoopThread = std::thread( &EventLoop::run, this); 
    }
    else{
        run();
    }

    return true;
}

void EventLoop::notify(const EventLoop::EventState state)
{    
    m_state |= static_cast<uint8_t>(state);
    uint64_t eventFdRequest = 1;
    ::write(m_eventFd, &eventFdRequest, sizeof(uint64_t));  
}

void EventLoop::subscribe(const std::string& signal, std::function<void (const std::shared_ptr<EventArgs>&)> slot)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_subscriberList.push_back( std::make_pair( signal, slot) );
}

void EventLoop::publish(const std::string& signal, const std::shared_ptr<EventArgs>& eventArgs)
{    
    std::lock_guard<std::mutex> lock(m_mutex);
    for(const auto& subscriber : m_subscriberList)
    {
        if(subscriber.first == signal)
        {            
            m_queue.emplace( subscriber.second, eventArgs);
        }
    }

    notify(EventLoop::EventState::QUEUE_READY);      
}

}
