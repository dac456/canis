#include "Engine.h"
#include "Renderer.h"
#include "GlShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "EntityManager.h"
#include "PluginManager.h"
#include "ScriptManager.h"
#include "MeshManager.h"

#include "IObject.h"

namespace Canis
{

    Engine::Engine(){
        _renderer = nullptr;
        
        _scriptManager = new ScriptManager();
        _shaderManager = new GlShaderManager();
        _textureManager = new TextureManager();
        _materialManager = new MaterialManager();
        _meshManager = new MeshManager();
        _entityManager = new EntityManager();
        _pluginManager = new PluginManager();

        _dynamicsEnabled = true;
    }

    Engine::~Engine(){
        delete _pluginManager;
        delete _entityManager;
        delete _meshManager;
        delete _materialManager;
        delete _textureManager;
        delete _shaderManager;
        
        if(_renderer != nullptr){
            delete _renderer;
        }
        
        delete _scriptManager;

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
