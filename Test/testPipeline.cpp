#include <iostream>

#include "../PipelineManager/EventManager.h"

//g++ -std=c++20 ../ModuleManager/Module*.cpp ../Module/Module*.cpp ../PipelineManager/*.cpp ../System/*.cpp testPipeline.cpp $(pkg-config --cflags --libs opencv4 jsoncpp)

int main(int argc, char const *argv[])
{
    EventManager eventManager;

    eventManager.runEventLoop();

    return 0;
}
