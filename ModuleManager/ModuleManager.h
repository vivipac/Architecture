#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>
#include "ModuleLoader.h"

      
class ModuleManager
{
    public:
    ModuleManager(const std::string& modulesPath = "/opt/fm/lib/modules");
       
    void addModule (const std::string& moduleName);

    void delModule (const std::string& moduleName);  

    ModuleLoader::ModulePtr getModule(const std::string& moduleName);

    private:
    std::string m_modulesPath;
    ModuleLoader m_moduleLoader;
    std::vector<std::pair<std::string, ModuleLoader::ModulePtr>> m_modules;

};


#endif