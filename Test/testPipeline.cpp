#include <iostream>

#include "PipelineManager.h"

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
