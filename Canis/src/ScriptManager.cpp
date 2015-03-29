#include "ScriptManager.h"
#include "Script.h"
#include "IScriptProvider.h"
#include "PythonProvider.h"

namespace Canis
{
    
    ScriptManager::ScriptManager(){
        _lastAccessed = nullptr;
        
        new PythonProvider();
    }
    
    ScriptManager::~ScriptManager(){
    }
    
    void ScriptManager::registerProvider(IScriptProvider* provider){
		provider->initialize(); //TODO: don't register if this fails
        _providers[provider->getType()] = provider;
        std::cout << "Registered Script Provider: " << provider->getType() << std::endl;
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
    
    void ScriptManager::runStep(Script* script){
		if(script != nullptr){
			_providers[script->getType()]->runStep(script);
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
