#include "TcpClient.h"

namespace vivi
{


TcpClient::TcpClient(std::string ip, short port, EventLoop & loop):
    m_socketFd(-1),
    m_ip(ip),
    m_port(port),
    m_loop(loop)
{    
    if( (m_socketFd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "socket error : " << strerror(errno) << std::endl;
        return ;
    }
    
    ::memset(&m_sockaddr, 0, sizeof(struct sockaddr_in) );

    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_addr.s_addr = inet_addr(m_ip.data());
    m_sockaddr.sin_port = htons(m_port);

}

TcpClient::~TcpClient()
{
    ::close(m_socketFd);
    //TODO : supprimer le watch uniquement si le fd est dans m_fds
    //m_loop.removeWatchFromEventLoop( m_socketFd ); 
}

bool TcpClient::connect()
{
    if( ::connect(m_socketFd, reinterpret_cast<struct sockaddr*>(&m_sockaddr), static_cast<socklen_t>(sizeof(struct sockaddr_in))) < 0 )
    {
        std::cout << "connect error : " << strerror(errno) << std::endl;
        return false;
    }

    return true;    
}

void TcpClient::addWatchToEventLoop(const std::function<void (void)>& inCallback )
{
    Watch w( m_socketFd, inCallback);
    m_loop.addWatchToEventLoop( w );
}

int TcpClient::sendTo(char* buffer, int size)
{
    return ::write(m_socketFd, buffer, size);
    
}

int TcpClient::recv(unsigned char* buffer, int size)
{
    return :: read( m_socketFd, buffer, size );
}


}