#include "PipelineController.h"
#include <fstream>

PipelineController::PipelineController(const std::string& pipelineConfig): 
    m_pipelineConfig(pipelineConfig)
{
    
}
       
std::vector<std::string> PipelineController::whoAreNext(const std::string& moduleName) const //TODO optimize this shit
{
    std::vector<std::string> moduleNameList;
    std::ifstream file(m_pipelineConfig);
    Json::Value root;
    file >> root;

    for (const auto& module : root)
    {
        std::string currentModuleName = module["module_name"].asString();
        Json::Value inputs = module["input"];

        if (inputs.isArray() && !inputs.empty())
        {
            for (const auto& input : inputs)
            {
                if (input.isString() && input.asString() == moduleName)
                {
                    moduleNameList.push_back(currentModuleName);
                    break;
                }
            }
        }
    }

    return moduleNameList;
}

std::vector<std::string> PipelineController::getAllModules() const
{
    std::vector<std::string> moduleNameList;
    std::ifstream file(m_pipelineConfig); 
    Json::Value root;
    file >> root;

    for (const auto& module : root)
    {
        std::string moduleName = module["module_name"].asString();
        moduleNameList.push_back(moduleName);
    }
    return moduleNameList;
}