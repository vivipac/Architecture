#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "EventLoop.h"
#include "Watch.h"

namespace vivi
{

class TcpClient
{
    public:
    TcpClient(std::string ip, short port, EventLoop & loop);

    ~TcpClient();  

    void addWatchToEventLoop(const std::function<void (void)>& inCallback );  

    template<class T_Obj>
    inline void addWatchToEventLoop(T_Obj* obj, void (T_Obj::*callback)(void)){
        addWatchToEventLoop( std::bind( callback, obj));
    }

    bool connect();    // permet de se connecter à l'adresse Ip et au port

    int sendTo(const char* buffer, int size);

    int recv(unsigned char* buffer, int size);

    private:    

    int m_socketFd;
    std::string m_ip;
    short m_port;
    EventLoop & m_loop;

    struct sockaddr_in m_sockaddr;
    


};


}

#endif