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
            glm::vec3 camPos = activeCamera->getAbsolutePosition();
            std::vector<LightPtr> lights = _activeScene->getLightsClosestToPoint(glm::vec4(camPos, 1.0f));            
            
            for(size_t i=0; i<_renderTargets.size(); i++){
                _renderTargets[i]->set();
                _activeScene->render(_renderTargets[i]->getCamera(), projectionMatrix);
                
                for(auto& it : _renderQueue){
                    _renderGroup(activeCamera->getTransform(), projectionMatrix, it, lights);
                }                
            }        
        
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glViewport(0, 0, _width, _height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            _activeScene->render(activeCamera, projectionMatrix); //TODO: pass in pointer to this renderer?
            
            for(auto& it : _renderQueue){
                _renderGroup(activeCamera->getTransform(), projectionMatrix, it, lights);
            }
            
            //_flushRenderQueue();
        }
    }
    
    void Renderer::resize(int w, int h){
        _width = w;
        _height = h;
    }

    size_t Renderer::enqueueRenderable(RenderablePtr renderable, size_t priority){
        if(_renderQueue.count(renderable->getMaterial()) > 0){
            return _renderQueue[renderable->getMaterial()]->enqueueRenderable(renderable);
        }
        else{
            RenderGroupPtr group = std::make_shared<RenderGroup>();
            _renderQueue[renderable->getMaterial()] = group;
            
            return _renderQueue[renderable->getMaterial()]->enqueueRenderable(renderable);
        }
    }
    
    void Renderer::enqueueRenderableList(RenderableList renderables, size_t priority){
        for(auto renderable : renderables){
            if(_renderQueue.count(renderable->getMaterial()) > 0){
                _renderQueue[renderable->getMaterial()]->enqueueRenderable(renderable);
            }
            else{
                RenderGroupPtr group = std::make_shared<RenderGroup>();
                _renderQueue[renderable->getMaterial()] = group;
                
                _renderQueue[renderable->getMaterial()]->enqueueRenderable(renderable);
            }    
        }    
    }
    
    void Renderer::updateRenderable(RenderablePtr renderable){
        _renderQueue[renderable->getMaterial()]->updateRenderable(renderable);
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
    

    void Renderer::_renderGroup(glm::mat4 viewMatrix, glm::mat4 projMatrix, std::pair<Material*, RenderGroupPtr> group, std::vector<LightPtr> lights){
        Material* mat = group.first;
        
        if(mat != nullptr){
            //Get lights
            glm::mat4 lpos = glm::mat4(0.0f);
            glm::mat4 lcol = glm::mat4(0.0f);
            glm::vec4 lrad = glm::vec4(0.0f);
            
            for(size_t i=0; i<lights.size(); i++){
                lpos[i] = glm::vec4(lights[i]->getAbsolutePosition(), 1.0f);
                lcol[i] = glm::vec4(lights[i]->getDiffuse(), 1.0f);
                lrad[i] = lights[i]->getRadius();
            }            
            
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
                
                t.passes[j].shader->setUniformMat4f("cs_LightPositions", lpos);
                t.passes[j].shader->setUniformMat4f("cs_LightColors", lcol);
                t.passes[j].shader->setUniformVec4f("cs_LightRadius", lrad);                  

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
                        std::vector<VertexObject*> vertexObjects = it.second->item;
                        //t.passes[j].shader->setUniformMat4f("cs_ModelMatrix", renderable->getTransform());
                        //t.passes[j].shader->setUniformMat3f("cs_NormalMatrix", renderable->normalMatrix);                                        
                    
                        //TODO: multiple objects per group/renderable exists for CSG map support
                        //      not needed (i think) for assimp and could probably be dropped
                        for(size_t k=0; k<vertexObjects.size(); k++){
                            if(vertexObjects[k]->getLightmap() != nullptr){
                                t.passes[j].shader->setUniform1i("cs_UseLightmap", true);
                                vertexObjects[k]->getLightmap()->use(shd);
                            }
                            else
                                t.passes[j].shader->setUniform1i("cs_UseLightmap", false);
                        
                            if(t.passes[j].blend){
                                glEnable(GL_BLEND);
                                glBlendFunc(t.passes[j].blendSrc, t.passes[j].blendDst);
                            }
                        
                            vertexObjects[k]->render(it.second->count, it.second->transArray/*, it.second->lightPositionArray, it.second->lightColorArray, it.second->lightRadiiArray*/);
                        
                            glDisable(GL_BLEND);
                        }
                        
                        //it.second->transforms.clear();
                        //it.second->count = 0;
                    }
                }
                
            }
            
        }    
    }
    
}
