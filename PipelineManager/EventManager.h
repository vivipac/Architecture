#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "../System/EventLoop.h"
#include "../ProxyManager/ProxyManager.h"
#include "PipelineManager.h"

      
class EventManager
{
    public:
    EventManager();
    
    void runEventLoop();
     
    private: 
    void pipelineInitialization();     

    void subscribersInitialization(); 

    void proxyInitialization();   
    
    std::shared_ptr<vivi::EventLoop> m_pEventLoop;
    PipelineManager m_pipelineManager;        
    ProxyManager m_proxy;;
    
                                                                              
};


#endif