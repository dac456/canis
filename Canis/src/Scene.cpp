#include "Scene.h"
#include "SceneNode.h"
#include "Engine.h"
#include "Camera.h"
#include "Light.h"

namespace Canis
{

    Scene::Scene(std::string name, glm::mat4 transform)
        : IObject("scene", transform){
        _name = name;
        _activeCamera = nullptr;

        //--Physics--//
        _broadphase = new btDbvtBroadphase();
        _collisionConfiguration = new btDefaultCollisionConfiguration();
        _collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration);
        _solver = new btSequentialImpulseConstraintSolver();
        _dynamicsWorld = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphase, _solver, _collisionConfiguration);
        _dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f)); //TODO: move elsewhere
    }

    Scene::~Scene(){
        for(int i=0; i<_dynamicsWorld->getNumCollisionObjects(); i++){
            btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
            _dynamicsWorld->removeCollisionObject(obj);
        }

        for(size_t i=0; i<_nodes.size(); i++)
            delete _nodes[i];

        //--Physics--//


        delete _dynamicsWorld;
        delete _solver;
        //delete _broadphase;
        delete _collisionDispatcher;
        delete _collisionConfiguration;
    }

    void Scene::render(Camera* activeCamera, glm::mat4 projectionMatrix){
        if(Engine::getSingleton().isDynamicsEnabled())
            _dynamicsWorld->stepSimulation(1.0f/60.0f);

        for(size_t i=0; i<_nodes.size(); i++){
            if(activeCamera != nullptr){
                _activeCamera = activeCamera; 
                _nodes[i]->render(projectionMatrix, _activeCamera->getTransform());
            }
        }
    }

    void Scene::reset(){
    }

    void Scene::addSceneNode(SceneNode* node){
        node->_parent = this;
        _nodes.push_back(node);
    }
    
    void Scene::setActiveCamera(Camera* camera){
        _activeCamera = camera;
    }
    
    Camera* Scene::getActiveCamera(){
        return _activeCamera;
    }

    std::vector<SceneNode*> Scene::getNodes(){
        return _nodes;
    }

    btDiscreteDynamicsWorld* Scene::getDynamicsWorld(){
        return _dynamicsWorld;
    }
    
    void Scene::_addLight(Light* light){
        _lights.push_back(light);
    }
    
    std::vector<Light*> Scene::getLightsClosestToPoint(glm::vec4 point){
        //TODO: we will eventually only want to sort lights that we can see
        for(size_t i=0; i<_lights.size(); i++){
            glm::vec4 lightPos = glm::vec4(_lights[i]->getAbsolutePosition(), 1.0f);
            //glm::vec4 lightDir = glm::normalize(lightPos - point);
            //glm::vec4 eyeDir = glm::normalize(glm::vec4(_activeCamera->getAbsolutePosition(), 1.0f) - point);

            //_lights[i]->_brightness = glm::dot(lightDir, eyeDir);
            //_lights[i]->_brightness = glm::length(point - lightPos);
            _lights[i]->_brightness = fabs(glm::length(lightPos - point));
            //printf("Brightness: %f\n", _lights[i]->_brightness);
        }

        std::sort(_lights.begin(), _lights.end());
        
        std::vector<Light*> ret;
        int count = 0;
        if(_lights.size() <= 4)
            count = _lights.size();
        else
            count = 4;
            
        for(int i=0; i<count; i++){
            ret.push_back(_lights[i]);
        }
        
        return ret;
    }    

}
