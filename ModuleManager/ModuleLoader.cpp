#include "ModuleLoader.h"
#include "errno.h"
#include "cstring"
#include <iostream>

ModuleLoader::ModuleLoader()
{}

void ModuleLoader::unload(const std::string& filename)
{ 
    if( m_filename_handle_map.contains(filename))
    {
        std::cout << "we unload the module " << filename << std::endl;
        ::dlclose( m_filename_handle_map[filename] );
    }
    else{
        throw filename + std::string( " does not exist");
    }
}

ModuleLoader::ModulePtr ModuleLoader::load(const std::string& filename)
{    
    char* error = nullptr;
    void* handle = ::dlopen(filename.c_str(), RTLD_NOW);    
    if (!handle) {          
        error = ::dlerror();      
        throw std::string(error);
    }    
    
    Module* (*loader) (void) = reinterpret_cast<Module* (*) (void)>( ::dlsym(handle, "loader"));

    if (loader == nullptr) {
        error = ::dlerror();
        ::dlclose(handle);
        throw std::string( error );
    }

    Module* module = loader();
    if (module == nullptr) {
        ::dlclose(handle);        
        throw std::string(  "error with operator new, " ) + ::strerror(errno) ;
    }
    
    m_filename_handle_map[filename] = handle;

    return ModulePtr(module);
}