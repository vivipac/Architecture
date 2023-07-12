#include "EventManager.h"

EventManager::EventManager():        
    m_pEventLoop( new vivi::EventLoop ),
    m_pipelineManager(m_pEventLoop),
    m_proxy("127.0.0.1", 60000, *m_pEventLoop, m_pipelineManager) 
{  
    subscribersInitialization();

    pipelineInitialization();     

    proxyInitialization();
    
}

void EventManager::pipelineInitialization()
{
    m_pipelineManager.loadModules();
    m_pipelineManager.initModules();
}

void EventManager::proxyInitialization()
{            
    m_proxy.initialization();
}

void EventManager::subscribersInitialization()
{
    m_pEventLoop->subscribe("next", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        m_pipelineManager.runModule(eventArgs);
    });

    // m_pEventLoop->subscribe("error", [this](const std::shared_ptr<EventArgs>& eventArgs)
    // {
    //     m_errorManager.errorManager(eventArgs);
    // });

}

void EventManager::runEventLoop()
{
    m_pEventLoop->runEventLoop();
}
