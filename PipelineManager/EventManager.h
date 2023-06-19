#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "../System/EventLoop.h"
#include "../System/TcpClient.h"
#include "PipelineManager.h"

      
class EventManager
{
    public:
    EventManager();
    
    void runEventLoop();
     
    private: 
    void pipelineInitialization();     

    void watchersInitialization();

    void subscribersInitialization(); 

    void proxyInitialization();

    void parseCommands();    
    
    PipelineManager m_pipelineManager;    
    std::shared_ptr<vivi::EventLoop> m_pEventLoop;
    vivi::TcpClient m_proxyClient;
    
                                                                              
};


#endif