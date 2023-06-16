#ifndef VISION_H
#define VISION_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../../Module/Module.h"
#include "../../Module/ModuleInfo.h"
#include "../../System/EventArgs.h"
#include "../../System/EventLoop.h"
     

//to create module.so : g++ -c -fpic Vision.cpp $(pkg-config --cflags --libs opencv4)&& g++ -shared Vision.o -o Vision.so && rm Vision.o

class Vision : public Module
{
    public:
        Vision();

        ~Vision() override = default;  

        const std::string& className() override;      

        void init(const Module::EventLoopPtr& eventLoop) override;
        
        void config(const Json::Value& config) override;

        void run(const std::shared_ptr<EventArgs>& inEventArgs = nullptr) override; 

    private:
    const std::string m_className;
    Module::EventLoopPtr m_eventLoopToPublish;
                                                                                       
};
FRONTMATEC_MODULE(Vision);

#endif