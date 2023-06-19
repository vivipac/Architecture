#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <string>
#include <jsoncpp/json/json.h>

#include "../Module/ModuleInfo.h"
#include "../ModuleManager/ModuleManager.h"
#include "../System/DefaultThreadPool.h"
#include "PipelineController.h"

      
class PipelineManager
{
    public:
    PipelineManager(const std::string& pipelineConfFilename = "./pipeline.json", const std::string& modulesDirectory = "./Modules");        

    void loadModules();

    void initModules(const Module::Ptr& eventLoop);   

    void updateModuleConfig(const Json::Value& config);

    void runModule(const std::shared_ptr<EventArgs>& eventArgs);


    private:        
    PipelineController m_pipelineController;
    ModuleManager m_moduleManager;
    Frontmatec::Libs::Common::DefaultThreadPool m_threadpool;
                                                                              
};


#endif