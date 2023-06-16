#ifndef ACQUISITION_H
#define ACQUISITION_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../../Module/Module.h"
#include "../../Module/ModuleInfo.h"
#include "../../System/EventArgs.h"
#include "../../System/EventLoop.h"
#include "../../System/Timer.h"
     

class Acquisition : public Module
{
    public:
        Acquisition();

        ~Acquisition() override = default;  

        const std::string& className() override;      

        void init(const Module::EventLoopPtr& eventLoop) override;
        
        void config(const Json::Value& config) override;

        void run(const std::shared_ptr<EventArgs>& inEventArgs = nullptr) override; 

    private:
    const std::string m_className;
    vivi::EventLoop m_eventLoop;
    vivi::Timer m_timer;
    Module::EventLoopPtr m_eventLoopToPublish;
                                                                                       
};
FRONTMATEC_MODULE(Acquisition);

#endif