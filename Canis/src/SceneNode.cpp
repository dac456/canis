#include "Scene.h"
#include "SceneNode.h"
#include "IEntity.h"
#include "Light.h"
#include "Camera.h"
#include "AssimpLoader.h"

namespace Canis
{

    SceneNode::SceneNode(std::string name, glm::mat4 transform)
        : IObject("node", transform) {
        _parent = nullptr;
        _name = name;
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

    void SceneNode::attachLight(Light* light){
        light->_parent = this;
        Scene* parentScene = this->getParentScene();
        if(parentScene != nullptr){
            parentScene->_addLight(light);
        }
        
        _lights.push_back(light);
    }

    void SceneNode::attachCamera(Camera* camera){
        camera->_parent = this;
        _cameras.push_back(camera);
    }
    
    void SceneNode::setName(std::string name){
        _name = name;
    }

    std::string SceneNode::getName(){
        return _name;
    }

    std::vector<SceneNode*> SceneNode::getChildren(){
        return _children;
    }

    std::vector<IEntity*> SceneNode::getEntities(){
        return _entities;
    }

    std::vector<Light*> SceneNode::getLights(){
        return _lights;
    }

    std::vector<Camera*> SceneNode::getCameras(){
        return _cameras;
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
