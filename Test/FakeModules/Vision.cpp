#include "Vision.h"

#include <iostream>

Vision::Vision():
    m_className("Vision")
{}

const std::string& Vision::className()
{
    return m_className;
}      

void Vision::init(const Module::EventLoopPtr& eventLoop) 
{
    m_eventLoopToPublish = eventLoop;
} 

void Vision::config(const Json::Value& config) 
{

} 

void Vision::run(const std::shared_ptr<EventArgs>& inEventArgs)
{
    std::shared_ptr<ModuleInfo> moduleInfo (new ModuleInfo(className()));


    std::shared_ptr<ModuleInfo> moduleInfoFromPreviousModule = std::static_pointer_cast<ModuleInfo>(inEventArgs);
    std::cout << "I am the Vision Module, and my previous module was " << moduleInfoFromPreviousModule->m_moduleName << std::endl;
    
    
    m_eventLoopToPublish->publish("next", moduleInfo);
} 
