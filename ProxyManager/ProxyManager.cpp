#include "ProxyManager.h"

ProxyManager::ProxyManager(const std::string& ip, const short port, ::vivi::EventLoop& loop, PipelineManager& pipelineManager):
    m_ip(ip),
    m_port(port),
    m_eventLoop(loop),
    m_pipelineManager(pipelineManager),
    m_pProxyClient( new vivi::TcpClient(m_ip, m_port, m_eventLoop) )
{
}

void ProxyManager::initialization()
{
    m_eventLoop.subscribe("proxyConnection", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        reconnectionToProxy();
    });

    m_pProxyClient->addWatchToEventLoop( [this](){        
        parseCommands();
    });

    if(!m_pProxyClient->connect())
    {
        std::cerr << "Impossible to connect to proxy" << std::endl;
        m_eventLoop.publish("proxyConnection");
        return;
    }
        
    char message[] = "Salut mon poulet\n"; //TODO libCommunication and send getAllConfig in Json format
    m_pProxyClient->sendTo( message , 18);
}

void ProxyManager::reconnectionToProxy()
{
    std::cout << "trying to connect again" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2)); //TODO set a timer of 3 sec into the event loop
    m_pProxyClient.reset(new vivi::TcpClient(m_ip, m_port, m_eventLoop));
    m_pProxyClient->addWatchToEventLoop( [this](){        
        parseCommands();
    });
    if(!m_pProxyClient->connect())
    {
        m_eventLoop.publish("proxyConnection");
    }            
}

void ProxyManager::parseCommands()
{
    unsigned char buffer[4096];//TODO avoir une fonction qui detect dans le message un format json et le découpe s'il y en a eu plusieur
    int bytesRead = m_pProxyClient->recv(buffer, sizeof(buffer));
    if(bytesRead <= 0)
    {
        std::cerr << "read returns error : " << std::strerror(errno) << std::endl;
        m_eventLoop.publish("proxyConnection");
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
            m_pProxyClient->sendTo( parseErrors.c_str() , parseErrors.size());
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
            //m_proxyClient.sendTo( "Command not recognized" , xxx.size());
            //m_pEventLoop->publish("error");
        }  
    }
    catch(...)
    {
        std::cerr << "exception caught" << std::endl;
    }
}