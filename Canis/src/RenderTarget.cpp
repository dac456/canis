#include "RenderTarget.h"
#include "Camera.h"
#include "FramebufferObject.h"

namespace Canis
{

    RenderTarget::RenderTarget(std::string name, Camera* camera, FramebufferObject* fbo){
        _name = name;
        _camera = camera;
        _fbo = fbo;
    }

    RenderTarget::~RenderTarget(){
        delete _fbo; //TODO: safe/necessary?
    }

    void RenderTarget::set(){
        //RenderManager::getSingleton().setViewMatrix(_camera->getTransform());
        //RenderManager::getSingleton().setViewVector(glm::vec4(_camera->getViewVector(), 1.0f));
        //RenderManager::getSingleton().setProjectionMatrix(RenderManager::getSingleton().getProjectionMatrix());
        //RenderManager::getSingleton().calculateFrustum(glm::mat4(1));

        _fbo->use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    
    Camera* RenderTarget::getCamera(){
        return _camera;
    }

}
