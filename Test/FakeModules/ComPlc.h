#ifndef COM_PLC_H
#define COM_PLC_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../../Module/Module.h"
#include "../../Module/ModuleInfo.h"
#include "../../System/EventArgs.h"
#include "../../System/EventLoop.h"

//to create module.so : g++ -c -fpic ComPlc.cpp $(pkg-config --cflags --libs opencv4)&& g++ -shared ComPlc.o -o ComPlc.so && rm ComPlc.o
    

class ComPlc : public Module
{
    public:
        ComPlc();

        ~ComPlc() override = default;  

        const std::string& className() override;      

        void init(const Module::EventLoopPtr& eventLoop) override;
        
        void config(const Json::Value& config) override;

        void run(const std::shared_ptr<EventArgs>& inEventArgs = nullptr) override; 

    private:
    const std::string m_className;
    Module::EventLoopPtr m_eventLoopToPublish;
                                                                                       
};
FRONTMATEC_MODULE(ComPlc);

#endif