#ifndef __RENDERMANAGER_H
#define __RENDERMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI RenderManager : public Singleton<RenderManager>{
    private:
        int _width, _height;

        glm::mat4 _viewMatrix, _projectionMatrix;
        glm::vec3 _viewPosition;
        glm::vec4 _viewVector;
        glm::vec4 _frustum[6];

        std::vector<RenderTarget*> _renderTargets;

        std::vector<Scene*> _scenes;
        std::vector<Light*> _lights;
        std::vector<Camera*> _cameras;

        Camera* _activeCamera;

    public:
        RenderManager();
        ~RenderManager();

        void update();
        void resize(int width, int height);

        void addRenderTarget(RenderTarget* target);

        void calculateFrustum(glm::mat4 transform);

        void addLight(Light* light);
        std::pair<std::pair<glm::mat4, glm::mat4>, glm::vec4> getLightsClosestToPoint(glm::vec4 point);

        void addScene(Scene* scene);
        std::vector<Scene*> getScenes();

        void addCamera(Camera* camera);
        void setActiveCamera(Camera* camera);
        Camera* getActiveCamera();

        void setViewMatrix(glm::mat4 viewMatrix);
        glm::mat4 getViewMatrix();
        void setProjectionMatrix(glm::mat4 projectionMatrix);
        glm::mat4 getProjectionMatrix();

        void setViewVector(glm::vec4 viewVector);
        glm::vec4 getViewVector();

        glm::vec4* getFrustum();
    };

}

#endif
