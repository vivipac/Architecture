#include <iostream>

#include "../PipelineManager/PipelineManager.h"

//g++ -std=c++20 ../ModuleManager/Module*.cpp ../Module/Module*.cpp ../PipelineManager/*.cpp ../System/*.cpp testPipeline.cpp $(pkg-config --cflags --libs opencv4 jsoncpp)

int main(int argc, char const *argv[])
{
    PipelineManager pipelineManager;

    pipelineManager.componentsInitialization();

    pipelineManager.subscribe();

    pipelineManager.loadModules();

    pipelineManager.initModules();

    pipelineManager.startPipeline();

    return 0;
}
