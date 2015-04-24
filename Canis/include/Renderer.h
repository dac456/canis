#ifndef __RENDERER_H
#define __RENDERER_H

#include "Common.h"

namespace Canis
{
    
    class CSAPI Renderer{
    private:
        std::vector<ScenePtr> _scenes;
        std::vector<RenderTarget*> _renderTargets;
        
        ScenePtr _activeScene;
        
        int _width, _height;
        
    public:
        Renderer();
        ~Renderer();
        
        void render(Camera* activeCamera, glm::mat4 projectionMatrix);
        void resize(int w, int h);
        
        void addScene(ScenePtr scene);
        void attachRenderTarget(RenderTarget* target);
        
        void setScene(std::string name);
        void setScene(size_t id);
        void setScene(ScenePtr scene);
        
        ScenePtr getActiveScene();
    };
    
}

#endif
