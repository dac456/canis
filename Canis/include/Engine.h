#ifndef __ENGINE_H
#define __ENGINE_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI Engine : public Singleton<Engine>{
    private:
        RenderManager* _renderManager;
        GlShaderManager* _shaderManager;
        TextureManager* _textureManager;
        MaterialManager* _materialManager;
        EntityManager* _entityManager;
        PluginManager* _pluginManager;

        bool _dynamicsEnabled;

    private:
        boost::signals2::signal<void ()> _sigReset;
        typedef boost::signals2::signal<void ()>::slot_type ResetEvent;

    public:
        Engine();
        ~Engine();

        void setDynamicsEnabled(bool enabled);
        bool isDynamicsEnabled();

        void reset();

    private:
        boost::signals2::connection _connectReset(const ResetEvent& e);

        friend class IObject;
    };

}

#endif
