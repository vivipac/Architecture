#include "ModuleManager.h"

ModuleManager::ModuleManager(const std::string& modulesPath = "/opt/fm/lib/modules"):
    m_modulesPath(modulesPath)
{

}
       
void ModuleManager::addModule (const std::string& moduleName)
{
    ModuleLoader::ModulePtr module = m_moduleLoader.load( m_modulesPath + "/" + moduleName);
    m_modules.push_back(std::make_pair(moduleName, nullptr));
}

void ModuleManager::delModule (const std::string& moduleName)
{
    for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
    {
        if( it->first == moduleName)
        {
            m_moduleLoader.unload( it->first );
            it = m_modules.erase(it);
            return;
        }
    }

    throw std::string( moduleName + " NOT FOUND");

} 

ModuleLoader::ModulePtr ModuleManager::getModule(const std::string& moduleName)
{
    auto module = std::find_if(m_modules.begin(), m_modules.end(), [&moduleName](const std::pair<std::string, ModuleLoader::ModulePtr>& pair)
    {
        return pair.first == moduleName;
    });

    if( module != m_modules.end())
    {
        return module->second;
    }

    return nullptr;
}