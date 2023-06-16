#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>

#include "../System/EventArgs.h"
#include "../System/EventLoop.h"

#define FRONTMATEC_MODULE(className) extern "C" Module* loader(){ return new className; }        

class Module
{
    public:

        virtual ~Module() = default;  

        virtual const std::string& className() = 0;      

        virtual void init(const vivi::EventLoop& eventLoop) = 0;
        
        virtual void config(const Json::Value& config) = 0;

        virtual void run(const std::shared_ptr<EventArgs>& inEventArgs) = 0; 
                                                                                   
};


#endif