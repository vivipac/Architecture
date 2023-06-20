#include <iostream>
#include <cxxabi.h>
#include <csignal>
#include <execinfo.h>

#include "../PipelineManager/EventManager.h"

//g++ -std=c++20 ../ModuleManager/Module*.cpp ../Module/Module*.cpp ../PipelineManager/*.cpp ../System/*.cpp testPipeline.cpp $(pkg-config --cflags --libs opencv4 jsoncpp)

void handleErrorSignal(int signal) {
    fprintf(stderr, "Error: ProcessManager -- Received signal '%d'.\n", signal);

    void* addressBuffer[64];
    const int size{backtrace(addressBuffer, sizeof(addressBuffer))};
    char** symbols{backtrace_symbols(addressBuffer, size)};

    for (size_t i{2}; i < size; ++i) { // Start at 2 to avoid displaying useless information from the trace.
        Dl_info info;

        if (dladdr(addressBuffer[i], &info)) {
            const char* path{info.dli_fname};
            const char* functionName{info.dli_sname};
            int status;

            char* demangledFunctionName{abi::__cxa_demangle(functionName,
                                                            nullptr,
                                                            nullptr,
                                                            &status)};

            if (status == 0) {
                fprintf(stderr, " %s(%s)\n", path, demangledFunctionName);
                free(demangledFunctionName);
            } else {
                fprintf(stderr, " %s\n", symbols[i]);
            }
        }
    }

    free(symbols);
    exit(signal);
}

int main(int argc, char const *argv[])
{
    std::signal(SIGABRT, handleErrorSignal);
    std::signal(SIGFPE, handleErrorSignal);
    std::signal(SIGILL, handleErrorSignal);
    std::signal(SIGSEGV, handleErrorSignal);

    EventManager eventManager;

    eventManager.runEventLoop();

    return 0;
}
