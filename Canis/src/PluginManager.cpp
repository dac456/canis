#include "PluginManager.h"
#include "DynamicLibrary.h"
#include "IPlugin.h"

namespace Canis
{

    PluginManager::PluginManager(){
    }

    PluginManager::~PluginManager(){
    }

    std::vector<IPlugin*> PluginManager::getPlugins(){
        return _plugins;
    }

    void PluginManager::loadPlugin(const std::string& name){
        DynamicLibrary* dynLib = new DynamicLibrary(name);
        dynLib->load();

        DLL_START_PLUGIN startFunc = (DLL_START_PLUGIN)dynLib->getSymbol("dllStart");
        if(startFunc)
            startFunc();
    }

    void PluginManager::installPlugin(IPlugin* plugin){
        _plugins.push_back(plugin);
        plugin->install();
        //TODO: initialise?
        printf(std::string("Plugin: "+plugin->getName()+" "+plugin->getVersion()+" installed.\n").c_str());
    }

    void PluginManager::uninstallPlugin(IPlugin* plugin){
        std::vector<IPlugin*>::iterator it;

        it = std::find(_plugins.begin(), _plugins.end(), plugin);
        if(it != _plugins.end()){
            _plugins.erase(it);
            plugin->uninstall();
        }
    }

    void PluginManager::unloadPlugin(const std::string& name){
        //TODO: finish unload sequence. requires DynamicLibraries to be stored by name
    }

}
