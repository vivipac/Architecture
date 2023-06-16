#include "PipelineManager.h"

PipelineManager::PipelineManager():
    m_pipelineController("./pipeline.json"),
    m_moduleManager("./Modules"),
    m_pEventLoop( new vivi::EventLoop )
{
    m_moduleNameList = m_pipelineController.getAllModules();
}

void PipelineManager::componentsInitialization()
{
    //add Watcher in the eventLoop
}

void PipelineManager::errorSlot(const std::shared_ptr<EventArgs>& eventArgs)
{
    //dynamic_cast EventArgs to ErrorInfo
    //log and communication with ErrorServer
}

void PipelineManager::nextSlot(const std::shared_ptr<EventArgs>& eventArgs)
{
    if( eventArgs )
    {
        std::shared_ptr<ModuleInfo> moduleInfo = std::dynamic_pointer_cast<ModuleInfo>(eventArgs);
        if( moduleInfo != nullptr)
        {
            const std::string& moduleName = moduleInfo->m_moduleName;
            const std::vector<std::string> modules = m_pipelineController.whoAreNext( moduleName );

            for(const auto & module: modules)
            {
                ModuleLoader::ModulePtr pModule = m_moduleManager.getModule( module );
                m_threadpool.add_task( [pModule, eventArgs](){
                    pModule->run(eventArgs);
                });
            }
            return;
        }
    }

    //error
    
}

void PipelineManager::subscribe()
{
    m_pEventLoop->subscribe("next", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {
        nextSlot(eventArgs);
    });

    m_pEventLoop->subscribe("error", [this](const std::shared_ptr<EventArgs>& eventArgs)
    {   
        errorSlot(eventArgs);
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
        pModule->init(m_pEventLoop);
    }
}

void PipelineManager::configureModules()
{

}

void PipelineManager::startPipeline()
{
    m_pEventLoop->runEventLoop();
}