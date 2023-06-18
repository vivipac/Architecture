#ifndef PIPELINE_CONTROLER_H
#define PIPELINE_CONTROLER_H

#include <string>
#include <vector>
#include <algorithm>
#include <jsoncpp/json/json.h>

      
class PipelineController
{
    public:
    PipelineController(const std::string& pipelineConfig);

    void checkConsistentPipeline() const;
       
    std::vector<std::string> whoAreNext(const std::string& moduleName) const;

    std::vector<std::string> getAllModules() const; 

    private:    
    std::string m_pipelineConfig;      
    Json::Value m_jsonRoot;                                                                       
};


#endif