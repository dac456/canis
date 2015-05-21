#ifndef __RENDERER_H
#define __RENDERER_H

#include "Common.h"
#include "Mesh.h"

namespace Canis
{
    
    typedef std::pair<Material*, std::vector<Renderable>> RenderGroup;
    
    struct Renderable{
        MeshGroup meshGroup; //TODO: replace with vector of VertexObject?
        glm::mat4 transform;
        glm::mat3 normalMatrix;
        
        glm::mat4 lightPositions;
        glm::mat4 lightColors;
        glm::vec4 lightRadii;
    };    
    
    class CSAPI Renderer{
    private:
        std::vector<ScenePtr> _scenes;
        std::vector<RenderTarget*> _renderTargets;
        
        ScenePtr _activeScene;
        std::map<Material*, std::vector<Renderable>> _renderQueue;
        
        int _width, _height;
        
    public:
        Renderer();
        ~Renderer();
        
        void render(Camera* activeCamera, glm::mat4 projectionMatrix);
        void resize(int w, int h);
        
        void queueRenderable(Material* material, Renderable renderable, size_t priority = 0);
        
        void addScene(ScenePtr scene);
        void attachRenderTarget(RenderTarget* target);
        
        void setScene(std::string name);
        void setScene(size_t id);
        void setScene(ScenePtr scene);
        
        ScenePtr getActiveScene();
        
    private:
        void _renderGroup(glm::mat4 viewMatrix, glm::mat4 projMatrix, RenderGroup group);
        void _flushRenderQueue();
    };
    
}

#endif
