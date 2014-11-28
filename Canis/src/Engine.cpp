#include "Engine.h"
#include "RenderManager.h"
#include "GlShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "EntityManager.h"
#include "PluginManager.h"

#include "IObject.h"

namespace Canis
{

    Engine::Engine(){
        _renderManager = new RenderManager();
        _shaderManager = new GlShaderManager();
        _textureManager = new TextureManager();
        _materialManager = new MaterialManager();
        _entityManager = new EntityManager();
        _pluginManager = new PluginManager();

        _dynamicsEnabled = true;
    }

    Engine::~Engine(){
        delete _pluginManager;
        delete _entityManager;
        delete _materialManager;
        delete _textureManager;
        delete _shaderManager;
        delete _renderManager;

#ifdef CANIS_PLATFORM_WIN32
        _CrtDumpMemoryLeaks();
#endif
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
