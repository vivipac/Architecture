#include "EventManager.h"

EventManager::EventManager():        
    m_pEventLoop( new vivi::EventLoop )
{  
    pipelineInitalization();     

    watchersInitalization();

    subscribersInitalization();
}

void EventManager::pipelineInitalization()
{
    m_pipelineManager.loadModules();
    m_pipelineManager.initModules( m_pEventLoop );
}

void EventManager::watchersInitalization()
{

}


void EventManager::subscribersInitalization()
{
    m_pEventLoop->subscribe("next", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        m_pipelineManager.runModule(eventArgs);
    });

    // m_pEventLoop->subscribe("error", [this](const std::shared_ptr<EventArgs>& eventArgs)
    // {
    //     m_errorManager.errorManage(eventArgs);
    // });

}

void EventManager::runEventLoop()
{
    m_pEventLoop->runEventLoop();
}


