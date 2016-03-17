#ifndef __RENDERER_H
#define __RENDERER_H

#include "Common.h"

namespace Canis
{
    
    typedef std::map<Material*, RenderGroupPtr> RenderGroupMap;
    
    class CSAPI Renderer{
    private:
        std::vector<ScenePtr> _scenes;
        std::vector<RenderTarget*> _renderTargets;
        
        ScenePtr _activeScene;
        RenderGroupMap _renderQueue;
        
        int _width, _height;
        
    public:
        Renderer();
        ~Renderer();
        
        void render(Camera* activeCamera, glm::mat4 projectionMatrix);
        void resize(int w, int h);
        
        size_t enqueueRenderable(RenderablePtr renderable, size_t priority = 0);
        void enqueueRenderableList(RenderableList renderables, size_t priority = 0);
        void updateRenderable(RenderablePtr renderable);
        
        void addScene(ScenePtr scene);
        void attachRenderTarget(RenderTarget* target);
        
        void setScene(std::string name);
        void setScene(size_t id);
        void setScene(ScenePtr scene);
        
        ScenePtr getActiveScene();
        
    private:
        void _renderGroup(glm::mat4 viewMatrix, glm::mat4 projMatrix, std::pair<Material*, RenderGroupPtr> group, std::vector<LightPtr> lights);
    };
    
}

#endif
