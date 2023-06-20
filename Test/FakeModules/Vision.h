#ifndef VISION_H
#define VISION_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../../Module/Module.h"
#include "../../Module/ModuleInfo.h"
#include "../../System/EventArgs.h"
#include "../../System/EventLoop.h"
     

//to create module.so : g++ -fpic -shared Vision.cpp ../../Module/ModuleInfo.cpp ../../System/*.cpp  $(pkg-config --cflags --libs opencv4) -o Vision.so

class Vision : public Module
{
    public:
        Vision();

        ~Vision() override = default;  

        const std::string& className() override;      

        void init(const Module::Ptr& eventLoop) override;
        
        void config(const Json::Value& config) override;

        void run(const std::shared_ptr<EventArgs>& inEventArgs = nullptr) override; 

    private:
    const std::string m_className;
    Module::Ptr m_eventLoopToPublish;
    std::mutex m_mutex;
                                                                                       
};
FRONTMATEC_MODULE(Vision);

#endif