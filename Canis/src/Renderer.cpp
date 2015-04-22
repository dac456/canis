#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"

namespace Canis
{
    
    Renderer::Renderer(){
        _activeScene = nullptr;
    }
    
    Renderer::~Renderer(){
        for(size_t i=0; i<_scenes.size(); i++){
            if(_scenes[i] != nullptr){
                delete _scenes[i];
            }
        }
        
        _scenes.clear();
        
        _activeScene = nullptr;
    }
    
    void Renderer::render(Camera* activeCamera, glm::mat4 projectionMatrix){
        if(_activeScene != nullptr){
            for(size_t i=0; i<_renderTargets.size(); i++){
                _renderTargets[i]->set();
                _activeScene->render(_renderTargets[i]->getCamera(), projectionMatrix);
            }        
        
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glViewport(0, 0, _width, _height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            _activeScene->render(activeCamera, projectionMatrix); //TODO: pass in pointer to this renderer?
        }
    }
    
    void Renderer::resize(int w, int h){
        _width = w;
        _height = h;
    }
    
    void Renderer::addScene(Scene* scene){
        if(std::find(_scenes.begin(), _scenes.end(), scene) == _scenes.end()){
            _scenes.push_back(scene);
        }
    }
    
    void Renderer::setScene(std::string name){
    }
    
    void Renderer::setScene(size_t id){
        _activeScene = _scenes[id];
    }
    
    void Renderer::setScene(Scene* scene){
        _activeScene = scene;
        std::cout << "Active scene: " << _activeScene->getName() << std::endl;
    }
    
    Scene* Renderer::getActiveScene(){
        return _activeScene;
    }
    
}
