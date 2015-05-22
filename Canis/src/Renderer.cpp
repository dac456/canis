#include "Renderer.h"
#include "RenderGroup.h"
#include "Renderable.h"
#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"

#include "MaterialManager.h"
#include "TextureManager.h"
#include "AxisAlignedBox.h"
#include "Material.h"
#include "GlShader.h"
#include "Texture.h"
#include "Light.h"
#include "VertexObject.h"

namespace Canis
{
    
    Renderer::Renderer(){
        _activeScene = nullptr;
        
        //_renderQueue[0] = std::make_shared<RenderQueue>();
    }
    
    Renderer::~Renderer(){
        _scenes.clear();
    }
    
    void Renderer::render(Camera* activeCamera, glm::mat4 projectionMatrix){
        if(_activeScene != nullptr){
            for(size_t i=0; i<_renderTargets.size(); i++){
                _renderTargets[i]->set();
                _activeScene->render(_renderTargets[i]->getCamera(), projectionMatrix);
                
                for(auto& it : _renderQueue){
                    _renderGroup(activeCamera->getTransform(), projectionMatrix, it);
                }                
            }        
        
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glViewport(0, 0, _width, _height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            _activeScene->render(activeCamera, projectionMatrix); //TODO: pass in pointer to this renderer?
            
            for(auto& it : _renderQueue){
                _renderGroup(activeCamera->getTransform(), projectionMatrix, it);
            }
            
            //_flushRenderQueue();
        }
    }
    
    void Renderer::resize(int w, int h){
        _width = w;
        _height = h;
    }
    
    void Renderer::queueRenderable(Material* material, RenderablePtr renderable, size_t priority){
        //_renderQueue[priority]->addRenderable(material, renderable);
        //_renderQueue[material].push_back(renderable);
        if(_renderQueue.count(material) > 0){
            _renderQueue[material]->queueRenderable(renderable);
        }
        else{
            RenderGroupPtr group = std::make_shared<RenderGroup>();
            _renderQueue[material] = group;
            
            _renderQueue[material]->queueRenderable(renderable);
        }
    }
    
    void Renderer::addScene(ScenePtr scene){
        if(std::find(_scenes.begin(), _scenes.end(), scene) == _scenes.end()){
            _scenes.push_back(scene);
        }
    }
    
    void Renderer::setScene(std::string name){
    }
    
    void Renderer::setScene(size_t id){
        _activeScene = _scenes[id];
    }
    
    void Renderer::setScene(ScenePtr scene){
        _activeScene = scene;
        std::cout << "Active scene: " << _activeScene->getName() << std::endl;
    }
    
    ScenePtr Renderer::getActiveScene(){
        return _activeScene;
    }
    
    void Renderer::_renderGroup(glm::mat4 viewMatrix, glm::mat4 projMatrix, std::pair<Material*, RenderGroupPtr> group){
        Material* mat = group.first;
        
        if(mat != nullptr){
            RenderGroupPtr renderGroup = group.second;
            
            Technique t = mat->getTechniques()[0];
            for(size_t j=0; j<t.passes.size(); j++){
                GLuint shd = t.passes[j].shader->getProgramId();
                t.passes[j].shader->use();
                t.passes[j].shader->setUniformMat4f("cs_ViewMatrix", viewMatrix);
                t.passes[j].shader->setUniformMat4f("cs_ProjectionMatrix", projMatrix);
                //t.passes[j].shader->setUniformMat4f("cs_ModelMatrix", renderable.transform);
                //t.passes[j].shader->setUniformMat3f("cs_NormalMatrix", glm::inverseTranspose(glm::mat3(viewMatrix)*glm::mat3(renderable.transform)));
                t.passes[j].shader->setUniformVec4f("cs_ViewVector", glm::inverse(viewMatrix)[2]);
                //t.passes[j].shader->setUniformMat4f("cs_LightPositions", renderable.lightPositions);
                //t.passes[j].shader->setUniformMat4f("cs_LightColors", renderable.lightColors);
                //t.passes[j].shader->setUniformVec4f("cs_LightRadius", renderable.lightRadii);
                t.passes[j].shader->setUniformVec4f("cs_DiffuseMaterial", t.passes[j].diffuse);

                if(t.passes[j].textures.size() > 0){
                    t.passes[j].shader->setUniform1i("cs_UseTexture", true);
                    for(size_t k=0; k<t.passes[j].textures.size(); k++){
                        Texture* tex = TextureManager::getSingleton().getTexture(t.passes[j].textures[k]);
                        tex->use(shd);
                    }
                }
                else
                    t.passes[j].shader->setUniform1i("cs_UseTexture", false);
                
                QueueItemMap groupItems = renderGroup->getQueueItemMap();    
                for(auto& it : groupItems){
                    if(it.second->count > 0){
                        RenderablePtr renderable = it.second->item;
                        t.passes[j].shader->setUniformMat4f("cs_ModelMatrix", renderable->getTransform());
                        //t.passes[j].shader->setUniformMat3f("cs_NormalMatrix", renderable->normalMatrix);
                        t.passes[j].shader->setUniformMat4f("cs_LightPositions", renderable->getLightPositions());
                        t.passes[j].shader->setUniformMat4f("cs_LightColors", renderable->getLightColors());
                        t.passes[j].shader->setUniformVec4f("cs_LightRadius", renderable->getLightRadii());                        
                    
                        for(size_t k=0; k<renderable->getVertexObjects().size(); k++){
                            if(renderable->getVertexObjects()[k]->getLightmap() != nullptr){
                                t.passes[j].shader->setUniform1i("cs_UseLightmap", true);
                                renderable->getVertexObjects()[k]->getLightmap()->use(shd);
                            }
                            else
                                t.passes[j].shader->setUniform1i("cs_UseLightmap", false);
                        
                            if(t.passes[j].blend){
                                glEnable(GL_BLEND);
                                glBlendFunc(t.passes[j].blendSrc, t.passes[j].blendDst);
                            }
                        
                            renderable->getVertexObjects()[k]->render();
                        
                            glDisable(GL_BLEND);
                        }
                        
                        it.second->count = 0;
                    }
                }
                
            }
            
        }    
    }
    
}
