#include "ScriptManager.h"
#include "Script.h"
#include "IScriptProvider.h"

namespace Canis
{
    
    ScriptManager::ScriptManager(){
        _lastAccessed = nullptr;
    }
    
    ScriptManager::~ScriptManager(){
    }
    
    void ScriptManager::addScript(Script* script){
        script->_id = _scripts.size(); //needed?
        _scripts.push_back(script);
    }
    
    void ScriptManager::execute(std::string name){
        Script* script = getScript(name);
        if(script != nullptr && _providers.find(script->getType()) != _providers.end()){
            _providers[script->getType()]->execute(script);
        }
    }
    
    Script* ScriptManager::getScript(std::string name){
        if(_lastAccessed->getName() == name){
            return _lastAccessed;
        }
        else{
            for(auto s : _scripts){
                if(s->getName() == name){
                    _lastAccessed = s;
                    return s;
                }
            }
        }
        
        return nullptr;
    }

}
