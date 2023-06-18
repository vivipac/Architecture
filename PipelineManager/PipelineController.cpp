#include "PipelineController.h"
#include <fstream>
#include <iostream>

PipelineController::PipelineController(const std::string& pipelineConfig): 
    m_pipelineConfig(pipelineConfig)
{
    try 
    {
        std::ifstream file(m_pipelineConfig);
        file >> m_jsonRoot;        
    } 
    catch (const std::ifstream::failure& e) 
    {
        std::cerr << "Failed to open or read the file: " << e.what() << std::endl;
        ::exit(EXIT_FAILURE);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "An error occurred while parsing the JSON: " << e.what() << std::endl;
        ::exit(EXIT_FAILURE);
    }

    try
    {
        checkConsistentPipeline(); 
    }
    catch(const std::string& error)
    {
        std::cerr << error << std::endl;
        ::exit(EXIT_FAILURE);
    }
}

void PipelineController::checkConsistentPipeline() const
{
    //TODO check if input and output are consistents, throw std::string("error in pipeline.json in ligne xxx")
}
       
std::vector<std::string> PipelineController::whoAreNext(const std::string& moduleName) const //TODO optimize this shit
{
    std::vector<std::string> moduleNameList;  

    for (const auto& module : m_jsonRoot)
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

    for (const auto& module : m_jsonRoot)
    {
        std::string moduleName = module["module_name"].asString();
        moduleNameList.push_back(moduleName);        
    }
    return moduleNameList;
}