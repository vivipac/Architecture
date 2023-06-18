#include "ComPlc.h"

#include <iostream>

ComPlc::ComPlc():
    m_className("ComPlc")
{}

const std::string& ComPlc::className()
{
    return m_className;
}      

void ComPlc::init(const Module::Ptr& eventLoop) 
{
    m_eventLoopToPublish = eventLoop;
} 

void ComPlc::config(const Json::Value& config) 
{

} 

void ComPlc::run(const std::shared_ptr<EventArgs>& inEventArgs)
{
    std::shared_ptr<ModuleInfo> moduleInfo (new ModuleInfo(className()));


    std::shared_ptr<ModuleInfo> moduleInfoFromPreviousModule = std::static_pointer_cast<ModuleInfo>(inEventArgs);
    std::cout << "I am the ComPlc Module, and my previous module was " << moduleInfoFromPreviousModule->m_moduleName << std::endl;
    
    
    m_eventLoopToPublish->publish("next", moduleInfo);
} 
