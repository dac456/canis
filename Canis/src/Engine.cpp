#include "Engine.h"
#include "Renderer.h"
#include "GlShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "EntityManager.h"
#include "PluginManager.h"
#include "ScriptManager.h"

#include "IObject.h"

namespace Canis
{

    Engine::Engine(){
        _renderer = nullptr;
        
        _shaderManager = new GlShaderManager();
        _textureManager = new TextureManager();
        _materialManager = new MaterialManager();
        _entityManager = new EntityManager();
        _pluginManager = new PluginManager();
        _scriptManager = new ScriptManager();

        _dynamicsEnabled = true;
    }

    Engine::~Engine(){
        delete _scriptManager;
        delete _pluginManager;
        delete _entityManager;
        delete _materialManager;
        delete _textureManager;
        delete _shaderManager;
        
        if(_renderer != nullptr){
            delete _renderer;
        }

#ifdef CANIS_PLATFORM_WIN32
        _CrtDumpMemoryLeaks();
#endif
    }
    
    void Engine::setRenderer(Renderer* renderer){
        _renderer = renderer;
    }
    
    Renderer* Engine::getRenderer(){
        return _renderer;
    }

    void Engine::setDynamicsEnabled(bool enabled){
        _dynamicsEnabled = enabled;
    }

    bool Engine::isDynamicsEnabled(){
        return _dynamicsEnabled;
    }

    void Engine::reset(){
        _sigReset();
    }

    boost::signals2::connection Engine::_connectReset(const ResetEvent& e){
        return _sigReset.connect(e);
    }

}
