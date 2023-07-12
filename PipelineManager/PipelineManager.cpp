#include "PipelineManager.h"

PipelineManager::PipelineManager(const Module::Ptr& eventLoop, const std::string& pipelineConfFilename, const std::string& modulesDirectory):
    m_eventLoop(eventLoop),
    m_pipelineController(pipelineConfFilename),
    m_moduleManager(modulesDirectory)    
{}

void PipelineManager::loadModules()
{    
    for(const auto & moduleName: m_pipelineController.getAllModules())
    {      
        std::cout << "we load Module " << moduleName << std::endl;  
        m_moduleManager.addModule(moduleName);
    }
}

void PipelineManager::initModules()
{
    for(const auto & moduleName: m_pipelineController.getAllModules())
    {        
        ModuleLoader::ModulePtr pModule = nullptr;
        try
        {
            pModule = m_moduleManager.getModule(moduleName);    
        }
        catch(const std::string& error)
        {
            std::cerr << error << '\n';
            ::exit(EXIT_FAILURE);
        }                    
        pModule->init(m_eventLoop);
    }
}

void PipelineManager::runModule(const std::shared_ptr<EventArgs>& eventArgs)
{    
    if( eventArgs )
    {
        const std::shared_ptr<ModuleInfo> moduleInfo = std::dynamic_pointer_cast<ModuleInfo>(eventArgs); 
        if( moduleInfo != nullptr)
        {
            const std::string& moduleName = moduleInfo->m_moduleName;            
            const std::vector<std::string> modules = m_pipelineController.whoAreNext( moduleName );
            std::cout << "we are searching from module : " << moduleName << ", size = " << modules.size() <<  std::endl;   
            ModuleLoader::ModulePtr pModule = nullptr;
            for(const auto & module: modules)
            {                                  
                try
                {
                    pModule = m_moduleManager.getModule(module);    
                }
                catch(const std::string& error)
                {
                    std::cerr << "nique ta mere : " << error << '\n';
                    ::exit(EXIT_FAILURE);
                }                
                
                m_threadpool.add_task( [pModule, eventArgs](){
                    pModule->run(eventArgs);
                });          
            }
            return;
        }
    }

    std::cerr << "eventArgs is incorrect" << '\n';
    //m_eventLoop->publish("error");
}

void PipelineManager::updateModuleConfig(const Json::Value& config)
{   
    if (!config.isMember("ModuleName") || !config.isMember("Data")) {
        std::cerr << "error in json format, ModuleName or Data does not exist" << '\n';
        //m_eventLoop->publish("error");
        return;
    }

    std::string moduleName = config["ModuleName"].asString();
    Json::Value dataObject = config["Data"];
                           
    ModuleLoader::ModulePtr pModule = nullptr;
                    
    try
    {
        pModule = m_moduleManager.getModule(moduleName);    
    }
    catch(const std::string& error)
    {
        std::cerr << error << '\n';
        ::exit(EXIT_FAILURE);
    }                
    
    m_threadpool.add_task( [pModule, dataObject](){
        pModule->config(dataObject);
    });             
    
}
