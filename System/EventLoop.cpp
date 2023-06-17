#include "EventLoop.h"
#include <memory>
#include <functional>

#include <string.h>
#include <errno.h>

//TODO add mutex

namespace vivi
{

EventLoop::EventLoop(): 
    m_fds(NULL),
    m_fdsCount(0),
    m_eventFd(-1),
    m_exitEventLoop( false ),
    m_state(static_cast<uint64_t>(EventLoop::EventState::NONE))
{    

    if ( (m_eventFd = eventfd(static_cast<unsigned long>(0), 0)) < 0) 
    {
        std::cout << "timerfd_create error : " << strerror(errno) << std::endl;
        return ;
    }
    Watch w (m_eventFd, std::bind(&EventLoop::eventNotify, this)); //TODO mettre une lambda
    addWatchToEventLoop( w );
    
}

bool EventLoop::removeWatchFromEventLoop( const Watch & watch )
{
    struct pollfd * tmp = nullptr;
    tmp = static_cast<struct pollfd * > ( ::malloc( sizeof( struct pollfd) * (m_fdsCount - 1) ) );

    for (size_t i = 0; i < m_fdsCount; i++)
    {
        if( m_fds[i].fd != watch.fd() )
        {
            tmp[i] = m_fds[i];
        }
    }      

    ::free(m_fds);
    m_fds = tmp;

    notify(EventLoop::EventState::FD_LIST_MODIFIED);

    return true; 
}

bool EventLoop::addWatchToEventLoop(  const ::vivi::Watch & watch)
{
    struct pollfd * tmp = nullptr;
    tmp = static_cast<struct pollfd * > ( ::malloc( sizeof( struct pollfd) * (m_fdsCount + 1) ) );

    ::memcpy(tmp, m_fds, m_fdsCount * sizeof( struct pollfd));
        
    tmp[m_fdsCount].fd = watch.fd();
    tmp[m_fdsCount].events = POLLIN | POLLHUP | POLLERR ;

    m_watchList.push_back( watch );

    m_fdsCount++;

    ::free(m_fds); //TODO possibilité de segfault avec le poll() 
    m_fds = tmp;

    notify(EventLoop::EventState::FD_LIST_MODIFIED);

    return true; 
}

void EventLoop::watchNotify(int fd)
{
    for( auto & watch : m_watchList)
    {
        if( watch.fd() == fd)
        {
            watch.callback();   
            if( !watch.needData() )   //on vide le buffer, TODO : il y a un syscall qui fait ca      
            {
                char bufferDump[1024];
                
                int readData;
                do{
                    readData = read(watch.fd(), bufferDump, 1024);
                }while( readData >= 1024);
                
            }
        }                   
    }    
}

void EventLoop::eventNotify()
{    
    uint64_t eventFdRequest = 1;
    ::read(m_eventFd, &eventFdRequest, sizeof(uint64_t));
    
    if( m_state & static_cast<uint64_t>(EventLoop::EventState::QUEUE_READY))
    {        
        m_state &= ~ (static_cast<uint64_t>(EventLoop::EventState::QUEUE_READY));

        while( m_queue.size() > 0 )
        {
            auto queue = m_queue.front(); 
            m_queue.pop();                

            queue.m_callback( queue.m_eventArgs);

        } 
    }
    if(m_state & static_cast<uint64_t>(EventLoop::EventState::FD_LIST_MODIFIED))
    {
        std::cout << "update fd list "  << std::endl;
        m_state &= ~ static_cast<uint64_t>(EventLoop::EventState::FD_LIST_MODIFIED);
    }

    if(m_state & static_cast<uint64_t>(EventLoop::EventState::QUIT_EVENT_LOOP))
    {
        std::cout << "quit event loop "  << std::endl;
        m_state &= ~ static_cast<uint64_t>(EventLoop::EventState::QUIT_EVENT_LOOP);

        m_exitEventLoop = true;
    }      
    
}

void EventLoop::run(){ //TODO adapter avec ce que l'on veut faire sur le schema de l archi

    while(!m_exitEventLoop)
    {        
        int nEvents = poll( m_fds, m_fdsCount, -1);
        //TODO je dois copier le m_fds et m_fdscount car il y a des risque de segfault si j ai un EventState::FD_LIST_MODIFIED
        for (size_t i = 0; i < m_fdsCount || nEvents > 0; i++)
        {
            if( m_fds[i].revents == POLLIN)
            {
                nEvents--;                
                                   
                watchNotify(m_fds[i].fd);
                
            }
        } 
    }
}

bool EventLoop::runEventLoop(bool isThread)
{
    if( isThread )
    {
        std::thread t( &EventLoop::run, this); //TODO dans le destructeur il faut tuer ce thread
        t.detach();
    }
    else{
        run();
    }

    return true;
}

void EventLoop::notify(const EventLoop::EventState state)
{
    m_state |= static_cast<uint64_t>(state);
    uint64_t eventFdRequest = 1;
    ::write(m_eventFd, &eventFdRequest, sizeof(uint64_t));  
}

void EventLoop::subscribe(const std::string& signal, std::function<void (const std::shared_ptr<EventArgs>&)> slot)
{
    m_subscriberList.push_back( std::make_pair( signal, slot) );
}

void EventLoop::publish(const std::string & signal, const std::shared_ptr<EventArgs>& eventArgs)
{    
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