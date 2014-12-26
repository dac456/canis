#ifndef __SCRIPTMANAGER_H
#define __SCRIPTMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI ScriptManager : public Singleton<ScriptManager>{
    private:
        Script* _lastAccessed;
        
        std::map<std::string, IScriptProvider*> _providers;
        std::vector<Script*> _scripts;
        
    public:
        ScriptManager();
        ~ScriptManager();
        
        void addScript(Script* script);
        void execute(std::string name);
        
        Script* getScript(std::string name);
    };

}

#endif
