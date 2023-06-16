#include "PipelineManager.h"

PipelineManager::PipelineManager()
{
    m_moduleNameList = m_pipelineController.getAllModules();
}

void PipelineManager::componentsInitialization()
{
    //add Watcher in the eventLoop
}

void PipelineManager::subscribe()
{
    m_eventLoop.subscribe("next", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        //dynamic_cast EventArgs to ModuleInfo
        std::string moduleName;
        //moduleName = moduleInfo.moduleName()
        std::vector<std::string> modules = m_pipelineController.whoAreNext( moduleName );

        for(const auto & module: modules)
        {
            ModuleLoader::ModulePtr pModule = m_moduleManager.getModule( module );
            m_threadpool.add_task( [&pModule, eventArgs](){
                pModule->run(eventArgs);
            });
        }

    });

    m_eventLoop.subscribe("error", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {   
        //dynamic_cast EventArgs to ErrorInfo
        //log and communication with ErrorServer
    });
}

void PipelineManager::loadModules()
{    
    for(const auto & moduleName: m_moduleNameList)
    {
        m_moduleManager.addModule(moduleName);
    }
}

void PipelineManager::initModules()
{
    for(const auto & moduleName: m_moduleNameList)
    {
        ModuleLoader::ModulePtr pModule = m_moduleManager.getModule(moduleName);
        pModule->init(m_eventLoop);
    }
}

void PipelineManager::configureModules()
{

}

void PipelineManager::startPipeline()
{
    m_eventLoop.runEventLoop();
}