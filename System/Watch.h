#ifndef WATCH_H
#define WATCH_H

#include <iostream>
#include <functional>


namespace vivi
{

class Watch 
{
    
    public:
        
        Watch(int inFd, const std::function<void (void)> &callback, bool needData = true);        
        
        int fd() const;        

        void callback(void);

        bool needData() const;

    private:
        int m_fd;        
        std::function<void (void)> m_callback;

        bool m_needData;//permet de savoir si la lecture du buffer via un ::read sera utilisé dans la callback
        

};

}


#endif