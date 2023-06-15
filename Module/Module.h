#ifndef MODULE_H
#define MODULE_H

#include <iostream>
#include <memory>
#include <Json>

#include "EventArgs.h"

#define FRONTMATEC_MODULE(className) extern "C" Module* loader(){ return new className; }        

class Module
{
    public:

        virtual ~Module() = default;  

        virtual const std::string& className() = 0;      

        virtual void init(const EventLoop& eventLoop) = 0;
        
        virtual void config(const Json& config) = 0;

        virtual void run(const std::shared_ptr<EventArgs>& inEventArgs) = 0; 
                                                                                   
};


#endif