#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "../System/EventLoop.h"
#include "PipelineManager.h"

      
class EventManager
{
    public:
    EventManager();
    
    void runEventLoop();
     
    private: 
    void pipelineInitalization();     

    void watchersInitalization();

    void subscribersInitalization();     
    
    PipelineManager m_pipelineManager;    
    std::shared_ptr<vivi::EventLoop> m_pEventLoop;
    
                                                                              
};


#endif