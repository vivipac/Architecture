#include "TcpClient.h"

namespace vivi
{


TcpClient::TcpClient(std::string ip, short port, EventLoop & loop):
    m_socketFd(-1),
    m_ip(ip),
    m_port(port),
    m_loop(loop),
    m_watch(-1, nullptr),
    m_isConnected(false)
{    
    if( (m_socketFd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "socket error : " << strerror(errno) << std::endl;
        return ;
    }
    std::cerr << "m_socketFd created is " << m_socketFd << std::endl;
    
    ::memset(&m_sockaddr, 0, sizeof(struct sockaddr_in) );

    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_addr.s_addr = inet_addr(m_ip.data());
    m_sockaddr.sin_port = htons(m_port);

}

TcpClient::~TcpClient()
{
    std::cout << "we delete an object of type TcpClient" << std::endl;
    removeWatchToEventLoop();
    //::close(m_socketFd); 
}

bool TcpClient::connect()
{
    if( ::connect(m_socketFd, reinterpret_cast<struct sockaddr*>(&m_sockaddr), static_cast<socklen_t>(sizeof(struct sockaddr_in))) < 0 )
    {
        std::cout << "connect error : " << strerror(errno) << std::endl;
        m_isConnected = false;
        return false;
    }
    m_isConnected = true;
    return true;    
}

bool TcpClient::isConnected()
{
    return m_isConnected;
}

void TcpClient::addWatchToEventLoop(const std::function<void (void)>& inCallback )
{
    m_watch = Watch( m_socketFd, inCallback);
    m_loop.addWatchToEventLoop( m_watch );
}

void TcpClient::removeWatchToEventLoop()
{
    m_loop.removeWatchFromEventLoop( m_watch );
}

int TcpClient::sendTo(const char* buffer, int size)
{
    return ::write(m_socketFd, buffer, size); 
}

int TcpClient::recv(unsigned char* buffer, int size)
{
    int bytesRead = ::read( m_socketFd, buffer, size ); //we are stucked here !!!!!!!!!!!!!! check with gdb. put async ?
    if(bytesRead <= 0)
    {
        m_isConnected = false;
    }
    return bytesRead;
}


}
