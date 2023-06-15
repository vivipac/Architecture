#include <iostream>

#include "EventLoop.h"

class Test{
    public :
    Test(vivi::EventLoop & eventLoop) :
        m_eventLoop( eventLoop )
    {        
    }
    void publish(void)
    {        
        char buffer[10];
        int byteRead = read( 0, buffer, 10);
        buffer[byteRead] = 0;
        m_data = buffer;       
        m_eventLoop.publish("next",  nullptr );
    }
    std::string data() const{
        return m_data;
    }
    private:
    vivi::EventLoop & m_eventLoop;
    std::string m_data;
    void * m_args[2];
};

int main(int argc, char const *argv[])
{
       
    vivi::EventLoop loop;

    loop.subscribe("next", [](const std::shared_ptr<EventArgs>& eventArgs){
        //we convert EventArgs to ModuleInfo
        //we get moduleName from moduleInfo
        //we put moduleName in m_moduleManager.whoAreNext( moduleName )
        // ... (read documentation newArchi)
        // threadpool ...
        std::cout << "signal next received, owner = " << ", null ? = " << (eventArgs != nullptr) << std::endl;
    });


    Test test(loop);  

    vivi::Watch w( 0, &test, &Test::publish);

    loop.addWatchToEventLoop( w );

    loop.publish("coco");

    return loop.runEventLoop();
}
