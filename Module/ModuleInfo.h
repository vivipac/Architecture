#ifndef MODULE_INFO_H
#define MODULE_INFO_H

#include <iostream>
#include <memory>
#include <opencv2/core/mat.hpp>

#include "../System/EventArgs.h"    

struct ModuleInfo : public EventArgs
{
    public:
    ModuleInfo(const std::string& className);
    ModuleInfo(const ModuleInfo& other) = delete;
    ModuleInfo(ModuleInfo&& other) = delete;
    ModuleInfo& operator=(const ModuleInfo& other) = delete;
    ModuleInfo& operator=(ModuleInfo&& other) = delete;
    ~ModuleInfo() override = default;

    std::string m_moduleName;
    unsigned long m_timestamp;
    cv::Mat m_mat;
                                                                                   
};


#endif