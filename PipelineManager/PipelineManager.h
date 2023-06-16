#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <string>
#include <vector>

#include "../Module/ModuleInfo.h"
#include "../ModuleManager/ModuleManager.h"
#include "../System/EventLoop.h"
#include "../System/DefaultThreadPool.h"
#include "PipelineController.h"

      
class PipelineManager
{
    public:
    PipelineManager();

    void componentsInitialization(); //connect with differents server (dataServer, loggingServer, commandServer)

    void subscribe(); //we subscribe for next and error signal

    void loadModules();

    void initModules();    

    private:
    void configureModules();

    void startPipeline();

    std::vector<std::string> m_moduleNameList;
    PipelineController m_pipelineController;
    ModuleManager m_moduleManager;
    std::shared_ptr<vivi::EventLoop> m_pEventLoop;
    Frontmatec::Libs::Common::DefaultThreadPool m_threadpool;
                                                                              
};


#endif