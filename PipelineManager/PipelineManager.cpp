#include "PipelineManager.h"

PipelineManager::PipelineManager(const std::string& pipelineConfFilename, const std::string& modulesDirectory):
    m_pipelineController(pipelineConfFilename),
    m_moduleManager(modulesDirectory)    
{    
}

void PipelineManager::loadModules()
{    
    for(const auto & moduleName: m_pipelineController.getAllModules())
    {      
        std::cout << "we load Module " << moduleName << std::endl;  
        m_moduleManager.addModule(moduleName);
    }
}

void PipelineManager::initModules(const Module::Ptr& eventLoop)
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
        pModule->init(eventLoop);
    }
}

void PipelineManager::runModule(const std::shared_ptr<EventArgs>& eventArgs)
{    
    if( eventArgs )
    {
        std::shared_ptr<ModuleInfo> moduleInfo = std::dynamic_pointer_cast<ModuleInfo>(eventArgs); 
        if( moduleInfo != nullptr)
        {
            const std::string& moduleName = moduleInfo->m_moduleName;            

            const std::vector<std::string> modules = m_pipelineController.whoAreNext( moduleName );

            ModuleLoader::ModulePtr pModule = nullptr;
            for(const auto & module: modules)
            {                     
                try
                {
                    pModule = m_moduleManager.getModule(module);    
                }
                catch(const std::string& error)
                {
                    std::cerr << error << '\n';
                    ::exit(EXIT_FAILURE);
                }                
                
                m_threadpool.add_task( [pModule, eventArgs](){
                    pModule->run(eventArgs);
                });              
            }
            return;
        }
    }

    //error
}

void PipelineManager::updateModuleConfig(const Json::Value& config)
{   
    //mettre exception et si error publish dans error 
    std::string moduleName = config["moduleName"].asString();
    Json::Value dataObject = config["data"];
                           
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
