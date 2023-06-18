#include "Acquisition.h"

Acquisition::Acquisition():
    m_className("Acquisition"),
    m_timer(5, m_eventLoop)
{}

const std::string& Acquisition::className()
{
    return m_className;
}      

void Acquisition::init(const Module::Ptr& eventLoop) 
{
    m_eventLoopToPublish = eventLoop;

    m_timer.addWatchToEventLoop([this](){
        run();
    });

    //run these functions when we receive startPipeline signal (subscribe in start signal)
    m_timer.start();
    m_eventLoop.runEventLoop(true); 
} 

void Acquisition::config(const Json::Value& config) 
{

} 

void Acquisition::run(const std::shared_ptr<EventArgs>& inEventArgs)
{
    std::shared_ptr<ModuleInfo> moduleInfo (new ModuleInfo(className()));

    //Do stuffs
    //cv::Mat mat(0)
    std::cout << "I am the Acquisition Module, I am a driver, nobody calls me !" << std::endl;

    
    m_eventLoopToPublish->publish("next", moduleInfo);
} 
