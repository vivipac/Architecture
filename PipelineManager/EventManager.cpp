#include "EventManager.h"

EventManager::EventManager():        
    m_pEventLoop( new vivi::EventLoop ),
    m_dataServerClient("127.0.0.1", 60000, *m_pEventLoop)
{  
    pipelineInitialization();     

    watchersInitialization();

    subscribersInitialization();
}

void EventManager::pipelineInitialization()
{
    m_pipelineManager.loadModules();
    m_pipelineManager.initModules( m_pEventLoop );
}

void EventManager::watchersInitialization()
{
    dataServerInitialization();
}

void EventManager::dataServerInitialization()
{            
    m_dataServerClient.addWatchToEventLoop( [this](){
        
        unsigned char buffer[256];
        
        int bytesRead = m_dataServerClient.recv(buffer, 256);
        buffer[bytesRead] = 0;

        std::cout << "message received = " << buffer << std::endl;
        //test if message is correct
        // Convert the buffer to a string
        try {
            std::string jsonString(reinterpret_cast<const char*>(buffer));
            // Parse the string as JSON
            Json::Value config;
            Json::CharReaderBuilder builder;
            std::istringstream jsonStream(jsonString); // Create an input stream
            std::string parseErrors;
            if (!Json::parseFromStream(builder, jsonStream, &config, &parseErrors)) {
                std::cerr << "Failed to parse the JSON string: " << parseErrors << std::endl;
                //on envoie un message d'erreur au serveur
                char message[] = "Error"; 
                m_dataServerClient.sendTo( message , sizeof(message));
                return;
            }
            m_pipelineManager.updateModuleConfig(config);

        } catch (const Json::Exception& e) {
            std::cerr << "Error json: " << e.what() << std::endl;
            char message[] = "Error"; 
            m_dataServerClient.sendTo( message , sizeof(message));            
        }

    });

    bool ret = m_dataServerClient.connect();
    if(!ret)
        std::cerr << "Impossible to connect to dataServer" << std::endl;

    char message[] = "Salut mon poulet\n"; //TODO libCommunication and send getAllConfig in Json format
    m_dataServerClient.sendTo( message , 18);
}


void EventManager::subscribersInitialization()
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


