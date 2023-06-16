#include "PipelineController.h"

PipelineController::PipelineController(const std::string& pipelineConfig): 
    m_pipelineConfig(pipelineConfig)
{
    designPipeline();
}

void PipelineController::designPipeline()
{

}
       
std::vector<std::string> PipelineController::whoAreNext(const std::string& moduleName) const 
{
    return {"coco", "toto"};
}

std::vector<std::string> PipelineController::getAllModules() const
{
    return {"coco", "toto"};
}