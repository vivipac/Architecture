#ifndef PROXY_MANAGER_H
#define PROXY_MANAGER_H

#include <string>
#include <jsoncpp/json/json.h>

#include "../System/EventLoop.h"
#include "../System/TcpClient.h"
#include "../PipelineManager/PipelineManager.h"

      
class ProxyManager
{
    public:
    ProxyManager(const std::string& ip, const short port, ::vivi::EventLoop& loop, PipelineManager& pipelineManager);    

    void initialization(); 

    void parseCommands(); 

    void reconnectionToProxy();  

    private: 
    std::string m_ip;
    short m_port;
    ::vivi::EventLoop & m_eventLoop;
    PipelineManager& m_pipelineManager;

    std::unique_ptr<vivi::TcpClient> m_pProxyClient;
                                                                              
};


#endif