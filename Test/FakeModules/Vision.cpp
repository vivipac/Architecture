#include "Vision.h"

#include <iostream>

Vision::Vision():
    m_className("Vision")
{}

const std::string& Vision::className()
{
    return m_className;
}      

void Vision::init(const Module::Ptr& eventLoop) 
{
    m_eventLoopToPublish = eventLoop;
} 

void Vision::config(const Json::Value& config) 
{
    std::cout << "I am Vision Module, and I received new parameters" << std::endl;
    // Convert it to a string
    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, config);
    std::cout << "data received = " << jsonString << std::endl;

    std::lock_guard<std::mutex> lock(m_mutex);

    int height = config["height"].asInt();
    int width = config["width"].asInt();
    std::string color = config["color"].asString();

    // Print the values
    std::cout << "height: " << height << "\n";
    std::cout << "width: " << width << "\n";
    std::cout << "color: " << color << "\n";
} 

void Vision::run(const std::shared_ptr<EventArgs>& inEventArgs)
{
    std::shared_ptr<ModuleInfo> moduleInfo (new ModuleInfo(className()));


    std::shared_ptr<ModuleInfo> moduleInfoFromPreviousModule = std::static_pointer_cast<ModuleInfo>(inEventArgs);
    std::cout << "I am the Vision Module, and my previous module was " << moduleInfoFromPreviousModule->m_moduleName << std::endl;
    
    
    m_eventLoopToPublish->publish("next", moduleInfo);
} 
