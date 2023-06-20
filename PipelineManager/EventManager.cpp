#include "EventManager.h"

EventManager::EventManager():        
    m_pEventLoop( new vivi::EventLoop ),
    m_pipelineManager(m_pEventLoop),
    m_proxyClient("127.0.0.1", 60000, *m_pEventLoop)
{  
    subscribersInitialization();

    pipelineInitialization();     

    watchersInitialization();
    
}

void EventManager::pipelineInitialization()
{
    m_pipelineManager.loadModules();
    m_pipelineManager.initModules();
}

void EventManager::watchersInitialization()
{
    proxyInitialization();
}

void EventManager::proxyInitialization()
{            
    m_proxyClient.addWatchToEventLoop( [this](){        
        parseCommands();
    });

    if(!m_proxyClient.connect())
    {
        std::cerr << "Impossible to connect to proxy" << std::endl;
        m_pEventLoop->publish("proxyConnection");
        return;
    }
        
    char message[] = "Salut mon poulet\n"; //TODO libCommunication and send getAllConfig in Json format
    m_proxyClient.sendTo( message , 18);
}

void EventManager::parseCommands()
{
    unsigned char buffer[4096];//TODO
        
    int bytesRead = m_proxyClient.recv(buffer, sizeof(buffer));
    if(bytesRead <= 0)
    {
        std::cerr << "read returns error : " << std::strerror(errno) << std::endl;
        m_pEventLoop->publish("proxyConnection");
        return;
    }
    buffer[bytesRead] = 0;

    std::cout << "message received = " << buffer << std::endl;
    
    try
    {
        std::string jsonString(reinterpret_cast<const char*>(buffer));
        Json::Value config;
        Json::CharReaderBuilder builder;
        std::istringstream jsonStream(jsonString); 
        std::string parseErrors;
        if (!Json::parseFromStream(builder, jsonStream, &config, &parseErrors)) {
            std::cerr << "Failed to parse the JSON string: " << parseErrors << std::endl;
            //send an error message to the proxy
            m_proxyClient.sendTo( parseErrors.c_str() , parseErrors.size());
            return;
        }

        if (!config.isMember("Command"))
        {
            std::cerr << "Command label is not found" << std::endl;
            //m_proxyClient.sendTo( parseErrors.c_str() , parseErrors.size());
            //m_pEventLoop->publish("error");
            return;
        }
        std::string command = config["Command"].asString();

        if(command == "updateData")
        {
            m_pipelineManager.updateModuleConfig(config);
        }
        else
        {
            std::cerr << "Command " << command << " not recognized" << std::endl;
            //m_proxyClient.sendTo( parseErrors.c_str() , parseErrors.size());
            //m_pEventLoop->publish("error");
        }  
    }
    catch(...)
    {
        std::cerr << "exception caught" << std::endl;
    }
           
}

void EventManager::subscribersInitialization()
{
    m_pEventLoop->subscribe("next", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        m_pipelineManager.runModule(eventArgs);
    });

    m_pEventLoop->subscribe("proxyConnection", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        std::cout << "trying to connect again" << std::endl;
        if(!m_proxyClient.connect())
        {
            m_pEventLoop->publish("proxyConnection");
        }
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
