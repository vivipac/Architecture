#include "Watch.h"

namespace vivi
{

Watch::Watch(int inFd, const std::function<void (void)> &inCallback, bool readDataNeed):    
    m_fd(inFd),        
    m_callback(inCallback),
    m_needData( readDataNeed )
{}

Watch::Watch(const Watch& watch)
{
    m_fd = watch.m_fd;
    m_callback = watch.m_callback;
    m_needData = watch.m_needData;
}

int Watch::fd() const
{
    return m_fd;
}

void  Watch::callback(void) 
{
    m_callback();       
}

bool Watch::needData() const
{
    return m_needData;
}

}