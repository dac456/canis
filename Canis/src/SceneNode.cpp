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
    }

    void SceneNode::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
        //glm::mat4 localTransform = _transform;
        //_marker->setTransform(_parent->getTransform()*localTransform);
        //_marker->render();
        //_marker->setTransform(localTransform);
        
        if(this->getScript() != nullptr){
            ScriptManager::getSingleton().run(this->getScript(), "step");
        }

        for(auto const& it : _entities){
            it.second->update(projectionMatrix, viewMatrix);
        }

        for(auto const& it : _children){
            it.second->render(projectionMatrix, viewMatrix);
        }
    }

    void SceneNode::reset(){
        _transform = _initialTransform;
    }

    void SceneNode::attachSceneNode(SceneNodePtr node){
        node->_parent = getptr();
        _children[node->getName()] = node;
    }

    void SceneNode::attachEntity(IEntityPtr entity){
        entity->_parent = getptr();
        entity->_entityAttached();
        _entities[entity->getName()] = entity;
    }

    void SceneNode::attachLight(LightPtr light){        
        light->_parent = getptr();
        ScenePtr parentScene = this->getParentScene();
        if(parentScene != nullptr){
            parentScene->_addLight(light);
        }
        
        _lights[light->getName()] = light;
    }

    void SceneNode::attachCamera(Camera* camera){
        camera->_parent = getptr();
        _cameras.push_back(camera);
    }
    
    void SceneNode::removeSceneNode(std::string name){
        _children.erase(name);
    }
    
    void SceneNode::removeEntity(std::string name){
        _entities.erase(name);
    }
    
    void SceneNode::removeLight(std::string name){
        ScenePtr parentScene = this->getParentScene();
        if(parentScene){
            parentScene->_removeLight(name);
        }
        
        _lights.erase(name);
    }

    std::vector<SceneNodePtr> SceneNode::getChildren(){
        std::vector<SceneNodePtr> children;
        std::transform( _children.begin(), _children.end(),
                   std::back_inserter(children),
                   boost::bind(&std::map<std::string, SceneNodePtr>::value_type::second,_1) );
                   
        return children;
    }

    std::vector<IEntityPtr> SceneNode::getEntities(){
        std::vector<IEntityPtr> entities;
        std::transform( _entities.begin(), _entities.end(),
                   std::back_inserter(entities),
                   boost::bind(&std::map<std::string, IEntityPtr>::value_type::second,_1) );
                   
        return entities;
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
    
    SceneNodePtr SceneNode::getChild(std::string name){
        try{
            return _children.at(name);
        }
        catch(const std::out_of_range& e){
            return nullptr;
        }
    }
    
    IEntityPtr SceneNode::getEntity(std::string name){
        try{
            return _entities.at(name);
        }
        catch(const std::out_of_range& e){
            return nullptr;
        }
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
        for(auto const& it: _entities){
            it.second->setScale(scale);
        }
        
        for(auto const& it : _children){
            it.second->setScale(scale);
        }
                
        //_transform = glm::scale(scale)*_transform;
        _scale = scale;
    }
    
    //TODO: just store a pointer directly to the parent scene?
    ScenePtr SceneNode::getParentScene(){
        if(_parent != nullptr){
            if(_parent->getType() == "scene"){
                return std::static_pointer_cast<Scene>(_parent);
            }
            else{
                IObjectPtr next = _parent->getParent();
                while(next != nullptr){
                    if(next->getType() == "scene"){
                        return std::static_pointer_cast<Scene>(next);
                    }
                }
            }
        }
        else{
            return nullptr;
        }
    }

}
