#include "Timer.h"
#include <string.h>
#include <errno.h>

#include "Watch.h"
namespace vivi
{

Timer::Timer(int interval, EventLoop & eventLoop):
    m_interval( interval ),
    m_eventLoop( eventLoop ),
    m_isTimerActivated(false),
    m_fd( -1 ) 
{

    if ( (m_fd = timerfd_create ( CLOCK_REALTIME, TFD_CLOEXEC)) < 0)
    {
        printf("timerfd_create error : %s\n", strerror(errno));
        return ;
    }

}

Timer::~Timer()
{
    if( m_fd != -1 )
    {
        removeWatchFromEventLoop();
        ::close ( m_fd );
    }
}

void Timer::addWatchToEventLoop(const std::function<void (void)> &inCallback )
{
    Watch w(m_fd, inCallback, false);
    m_eventLoop.addWatchToEventLoop( w );
}

void Timer::start(int delay_sec)
{
    m_isTimerActivated = true;

    if( delay_sec == 0 )// delay_sec must be superior to 0 to activate the timer
    {
        delay_sec = m_interval;
    }

    struct timespec tspec_period, tspec_expiration;

    ::memset( &tspec_period, 0, sizeof(struct timespec));
    ::memset( &tspec_expiration, 0, sizeof(struct timespec));


    tspec_period.tv_sec = m_interval;
    tspec_expiration.tv_sec = delay_sec;

    struct itimerspec t1;
    t1.it_value = tspec_expiration;
    t1.it_interval = tspec_period;

    if( timerfd_settime(m_fd, TFD_TIMER_ABSTIME, &t1, NULL) < 0)
    {
        printf( "timerfd_settime error : %s\n",  strerror(errno) );
        return;
    }
}

void Timer::stop()
{
    m_isTimerActivated = false;

    struct itimerspec t_stopTimer;

    ::memset( &t_stopTimer, 0, sizeof(struct itimerspec));

    if( timerfd_settime(m_fd, 0, &t_stopTimer, NULL) < 0)
    {
        printf( "timerfd_settime error : %s\n",  strerror(errno) );
        return;
    }
}

bool Timer::isTimerActivated() const
{
    return m_isTimerActivated;
}

void Timer::removeWatchFromEventLoop()
{
    m_eventLoop.removeWatchFromEventLoop( m_fd );
}


}