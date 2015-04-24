#include "Scene.h"
#include "SceneNode.h"
#include "Engine.h"
#include "Camera.h"
#include "Light.h"

namespace Canis
{

    Scene::Scene(std::string name, glm::mat4 transform)
        : IObject(name, "scene", transform){
        _activeCamera = nullptr;
        std::cout << "created world " << name << std::endl;

        //--Physics--//
        _broadphase = new btDbvtBroadphase();
        _collisionConfiguration = new btDefaultCollisionConfiguration();
        _collisionDispatcher = new btCollisionDispatcher(_collisionConfiguration);
        _solver = new btSequentialImpulseConstraintSolver();
        _dynamicsWorld = new btDiscreteDynamicsWorld(_collisionDispatcher, _broadphase, _solver, _collisionConfiguration);
        _dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f)); //TODO: move elsewhere
        
        _timer = new Timer();
        _lastTime = _timer->millis();
    }

    Scene::~Scene(){
        std::cout << "deleting world " << getName() << std::endl;
        for(int i=0; i<_dynamicsWorld->getNumCollisionObjects(); i++){
            btCollisionObject* obj = _dynamicsWorld->getCollisionObjectArray()[i];
            _dynamicsWorld->removeCollisionObject(obj);
        }

        //--Physics--//


        delete _dynamicsWorld;
        delete _solver;
        //delete _broadphase;
        delete _collisionDispatcher;
        delete _collisionConfiguration;
        delete _broadphase;
    }

    void Scene::render(Camera* activeCamera, glm::mat4 projectionMatrix){
        if(Engine::getSingleton().isDynamicsEnabled()){
            _dynamicsWorld->stepSimulation((float)(_timer->millis() - _lastTime)/1000.0f, 7);
        }
        _lastTime = _timer->millis();

        /*for(size_t i=0; i<_nodes.size(); i++){
            if(activeCamera != nullptr){
                _activeCamera = activeCamera; 
                _nodes[i]->render(projectionMatrix, _activeCamera->getTransform());
            }
        }*/
        for(auto const& it : _nodes){
            if(activeCamera != nullptr){
                _activeCamera = activeCamera;
                it.second->render(projectionMatrix, _activeCamera->getTransform());
            }
        }
    }

    void Scene::reset(){
        _solver->reset();
        _dynamicsWorld->clearForces();
        _broadphase->resetPool(_collisionDispatcher);
        
        btOverlappingPairCache* pairCache = _broadphase->getOverlappingPairCache();
        btBroadphasePairArray& pairArray = pairCache->getOverlappingPairArray();
        for(size_t i=0; i<pairArray.size(); i++){
            pairCache->cleanOverlappingPair(pairArray[i], _collisionDispatcher);
        }
    }

    void Scene::addSceneNode(SceneNodePtr node){
        node->_parent = getptr();
        //_nodes.push_back(node);
        _nodes[node->getName()] = node;
    }
    
    void Scene::removeSceneNode(std::string name){
        //_nodes.erase(std::remove(_nodes.begin(), _nodes.end(), node), _nodes.end());
        if(_nodes.count(name) != 0){
            _nodes.erase(name);
        }
    }
    
    void Scene::setActiveCamera(Camera* camera){
        _activeCamera = camera;
    }
    
    Camera* Scene::getActiveCamera(){
        return _activeCamera;
    }

    //TODO: deep search?
    SceneNodePtr Scene::getNode(std::string name){
        try{
            return _nodes.at(name);
        }
        catch(const std::out_of_range& e){
            return nullptr;
        }
    }

    std::vector<SceneNodePtr> Scene::getNodes(){
        std::vector<SceneNodePtr> children;
        std::transform( _nodes.begin(), _nodes.end(),
                   std::back_inserter(children),
                   boost::bind(&std::map<std::string, SceneNodePtr>::value_type::second,_1) );
                   
        return children;
    }

    btDiscreteDynamicsWorld* Scene::getDynamicsWorld(){
        return _dynamicsWorld;
    }
    
    void Scene::_addLight(LightPtr light){
        //_lights.push_back(light);
        _lights[light->getName()] = light;
    }
    
    void Scene::_removeLight(std::string name){
        //_lights.erase(std::remove(_lights.begin(), _lights.end(), light), _lights.end());
        if(_lights.count(name) != 0){
            _lights.erase(name);
        }        
    }
    
    std::vector<LightPtr> Scene::getLightsClosestToPoint(glm::vec4 point){
        //TODO: we will eventually only want to sort lights that we can see
        std::vector<LightPtr> lights;
        std::transform( _lights.begin(), _lights.end(),
                   std::back_inserter(lights),
                   boost::bind(&std::map<std::string, LightPtr>::value_type::second,_1) );        
        
        for(size_t i=0; i<lights.size(); i++){
            glm::vec4 lightPos = glm::vec4(lights[i]->getAbsolutePosition(), 1.0f);
            //glm::vec4 lightDir = glm::normalize(lightPos - point);
            //glm::vec4 eyeDir = glm::normalize(glm::vec4(_activeCamera->getAbsolutePosition(), 1.0f) - point);

            //_lights[i]->_brightness = glm::dot(lightDir, eyeDir);
            //_lights[i]->_brightness = glm::length(point - lightPos);
            lights[i]->_brightness = fabs(glm::length(lightPos - point));
            //printf("Brightness: %f\n", _lights[i]->_brightness);
        }

        std::sort(lights.begin(), lights.end());
        
        std::vector<LightPtr> ret;
        int count = 0;
        if(lights.size() <= 4)
            count = lights.size();
        else
            count = 4;
            
        for(int i=0; i<count; i++){
            ret.push_back(lights[i]);
        }
        
        return ret;
    }    

}
