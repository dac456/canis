#ifndef __PLUGINMANAGER_H
#define __PLUGINMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    typedef void (*DLL_START_PLUGIN)(void);
    typedef void (*DLL_STOP_PLUGIN)(void);

    class CSAPI PluginManager : public Singleton<PluginManager>{
    private:
        std::vector<IPlugin*> _plugins; //TODO: necessary?

    public:
        PluginManager();
        ~PluginManager();

        std::vector<IPlugin*> getPlugins();

        void loadPlugin(const std::string& name);
        void installPlugin(IPlugin* plugin);

        void uninstallPlugin(IPlugin* plugin);
        void unloadPlugin(const std::string& name);
    };

}

#endif
