#include <iostream>

#include "TcpClient.h"

int main(int argc, char const *argv[])
{
    
    
    vivi::EventLoop loop;
    vivi::TcpClient tcpClient("127.0.0.1", 55000, loop);

    
    tcpClient.addWatchToEventLoop( [&](){
        
        unsigned char buffer[256];
        
        int bytesRead = tcpClient.recv(buffer, 256);
        buffer[bytesRead] = 0;

        std::cout << "message received = " << buffer << std::endl;

    });

    tcpClient.connect();

    char message[] = "Salut mon poulet\n";
    tcpClient.sendTo( message , 18);


    loop.runEventLoop( );


    return 0;
}
