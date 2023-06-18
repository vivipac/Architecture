#ifndef COM_PLC_H
#define COM_PLC_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../../Module/Module.h"
#include "../../Module/ModuleInfo.h"
#include "../../System/EventArgs.h"
#include "../../System/EventLoop.h"

//to create module.so : g++ -fpic -shared ComPlc.cpp ../../Module/ModuleInfo.cpp ../../System/*.cpp  $(pkg-config --cflags --libs opencv4) -o ComPlc.so
    

class ComPlc : public Module
{
    public:
        ComPlc();

        ~ComPlc() override = default;  

        const std::string& className() override;      

        void init(const Module::Ptr& eventLoop) override;
        
        void config(const Json::Value& config) override;

        void run(const std::shared_ptr<EventArgs>& inEventArgs = nullptr) override; 

    private:
    const std::string m_className;
    Module::Ptr m_eventLoopToPublish;
                                                                                       
};
FRONTMATEC_MODULE(ComPlc);

#endif