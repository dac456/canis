#include "Scene.h"
#include "SceneNode.h"
#include "IEntity.h"
#include "Light.h"
#include "Camera.h"
#include "AssimpLoader.h"
#include "ScriptManager.h"

namespace Canis
{

    SceneNode::SceneNode(std::string name, glm::mat4 transform)
        : IObject(name, "node", transform) {
        _parent = nullptr;
        //_marker = new Mesh(new AssimpLoader("./Media/Models/marker.ms3d"));
        //_marker->setTransform(transform);
    }

    SceneNode::~SceneNode(){
        for(size_t i=0; i<_children.size(); i++)
            delete _children[i];

        for(size_t i=0; i<_entities.size(); i++)
            delete _entities[i];
        //delete _marker;
    }

    void SceneNode::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
        //glm::mat4 localTransform = _transform;
        //_marker->setTransform(_parent->getTransform()*localTransform);
        //_marker->render();
        //_marker->setTransform(localTransform);
        
        if(this->getScript() != nullptr){
            ScriptManager::getSingleton().run(this->getScript(), "step");
        }

        for(size_t i=0; i<_entities.size(); i++){
            _entities[i]->update(projectionMatrix, viewMatrix);
        }

        for(size_t i=0; i<_children.size(); i++)
            _children[i]->render(projectionMatrix, viewMatrix);
    }

    void SceneNode::reset(){
        _transform = _initialTransform;
    }

    void SceneNode::attachSceneNode(SceneNode* node){
        node->_parent = this;
        _children.push_back(node);
    }

    void SceneNode::attachEntity(IEntity* entity){
        entity->_parent = this;
        entity->_entityAttached();
        _entities.push_back(entity);
    }

    void SceneNode::attachLight(LightPtr light){
        /*auto search = _lights.find(light->getName());
        if(search != _lights.end()){
            Scene* parentScene = this->getParentScene();
            if(parentScene){
                parentScene->_removeLight(search->second);
            }            
        }*/
        
        light->_parent = this;
        Scene* parentScene = this->getParentScene();
        if(parentScene != nullptr){
            parentScene->_addLight(light);
        }
        
        //_lights.push_back(light);
        _lights[light->getName()] = light;
    }

    void SceneNode::attachCamera(Camera* camera){
        camera->_parent = this;
        _cameras.push_back(camera);
    }
    
    void SceneNode::removeEntity(IEntity* entity){
        if(std::find(_entities.begin(), _entities.end(), entity) != _entities.end()){
            _entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
        }
    }
    
    void SceneNode::removeLight(std::string name){
        Scene* parentScene = this->getParentScene();
        if(parentScene){
            parentScene->_removeLight(name);
        }
        
        if(_lights.count(name) != 0){
            _lights.erase(name);
        }
    }

    std::vector<SceneNode*> SceneNode::getChildren(){
        return _children;
    }

    std::vector<IEntity*> SceneNode::getEntities(){
        return _entities;
    }

    std::vector<LightPtr> SceneNode::getLights(){
        //return _lights;
        std::vector<LightPtr> lights;
        std::transform( _lights.begin(), _lights.end(),
                   std::back_inserter(lights),
                   boost::bind(&std::map<std::string, LightPtr>::value_type::second,_1) );
        
        return lights;
    }

    std::vector<Camera*> SceneNode::getCameras(){
        return _cameras;
    }
    
    LightPtr SceneNode::getLight(std::string name){
        try{
            return _lights.at(name);
        }
        catch(const std::out_of_range& e){
            return nullptr;
        }
    }
    
    //Move to IObject?
    void SceneNode::translate(glm::vec3 translation){
        _transform = glm::translate(translation)*_transform;
    }

    //Move to IObject?
    void SceneNode::setScale(glm::vec3 scale){
        for(auto e : _entities){
            e->setScale(scale);
        }
        
        for(auto n : _children){
            n->setScale(scale);
        }
                
        //_transform = glm::scale(scale)*_transform;
        _scale = scale;
    }
    
    //TODO: just store a pointer directly to the parent scene?
    Scene* SceneNode::getParentScene(){
        if(_parent != nullptr){
            if(_parent->getType() == "scene"){
                return static_cast<Scene*>(_parent);
            }
            else{
                IObject* next = _parent->getParent();
                while(next != nullptr){
                    if(next->getType() == "scene"){
                        return static_cast<Scene*>(next);
                    }
                }
            }
        }
        else{
            return nullptr;
        }
    }

}
