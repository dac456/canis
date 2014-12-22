#include "Prop.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Engine.h"

namespace Canis
{

    Prop::Prop(std::string name, Mesh* mesh, COLLISION_SHAPE shape, float mass)
        : IEntity("Prop", name, glm::mat4(1.0f), true){
            _mesh = mesh;
            _mesh->setTransform(glm::mat4(1.0f));
            _mass = mass;
            
            _paramTypes["name"] = PARAM_STRING;
            _paramTypes["mesh"] = PARAM_PATH;
            _paramTypes["shape"] = PARAM_SHAPE;
            _paramTypes["mass"] = PARAM_REAL;
            
            _params["name"] = name;
            _params["mass"] = std::to_string(mass);
            
            _shape = shape;

            if(shape == SPHERE_SHAPE){
                btVector3* p = new btVector3(0.0f, 0.0f, 0.0f);
                btScalar* r = new btScalar(_mesh->getBoundingBox().getRadius());
                _collisionShape = new btMultiSphereShape(p, r, 1);
            }
            else if(shape == BOX_SHAPE)
                _collisionShape = new btBoxShape(btVector3(_mesh->getBoundingBox().getMax().x, _mesh->getBoundingBox().getMax().y, _mesh->getBoundingBox().getMax().z));

            _rigidBody = nullptr;
    }

    Prop::~Prop(){
        delete _mesh;

        delete _rigidBody;
        delete _collisionShape;
    }

    void Prop::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
        glm::mat4 localTransform = _transform;
        //glm::mat4 meshTransform = _mesh->getTransform();

        if(_parent != nullptr){
            glm::mat4 absTransform = _parent->getTransform()*_transform;
            IObject* next = _parent->getParent();
            while(next != nullptr){
                absTransform = next->getTransform()*absTransform;
                next = next->getParent();
            }
            _mesh->setTransform(absTransform*glm::scale(_scale));
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(absTransform));
            //_rigidBody->setWorldTransform(_dynamicsTransform);
        }
        else{
            _mesh->setTransform(localTransform*glm::scale(_scale));
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(localTransform));
            //_rigidBody->setWorldTransform(_dynamicsTransform);
        }

        std::vector<Light*>  lights;
        SceneNode* parentNode = this->getParentNode();
        if(parentNode != nullptr){
            Scene* parentScene = parentNode->getParentScene();
            if(parentScene != nullptr){
                lights = parentScene->getLightsClosestToPoint(this->getAbsoluteTransform()[3]);
            }
        }
        _mesh->render(projectionMatrix, viewMatrix, lights);
        //_mesh->setTransform(meshTransform);
        
        //Update parameters
        for(auto p : _paramUpdateList){
            if(p == "mass"){
                if(_params["mass"] != ""){
                    _mass = std::stof(_params["mass"]);
                }
                else{
                    _mass = 0.000001f;
                }
                
                btVector3 inertia(0,0,0);
                _collisionShape->calculateLocalInertia(_mass, inertia);                
                _rigidBody->setMassProps(_mass, inertia);
                _paramUpdateList.pop_back();
            }
        }
    }

    void Prop::reset(){
        if(_parent != nullptr){
            //_rigidBody->setActivationState(DISABLE_DEACTIVATION);
            
            _dynamicsWorld->removeRigidBody(_rigidBody);
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_parent->getInitialTransform()));
            _rigidBody->clearForces();
            _rigidBody->setLinearVelocity(btVector3(0,0,0));
            _rigidBody->setAngularVelocity(btVector3(0,0,0));
            _rigidBody->setWorldTransform(_dynamicsTransform);
            _dynamicsWorld->addRigidBody(_rigidBody);
            
            //_rigidBody->setActivationState(ENABLE_DEACTIVATION);
        }
        
        if(_needsRebuild){
           if(_dynamicsWorld && _rigidBody){
               _dynamicsWorld->removeRigidBody(_rigidBody);
               delete _rigidBody->getCollisionShape();
               delete _rigidBody;
               
                if(_shape == SPHERE_SHAPE){
                    btVector3* p = new btVector3(0.0f, 0.0f, 0.0f);
                    btScalar* r = new btScalar(_mesh->getBoundingBox().getRadius());
                    _collisionShape = new btMultiSphereShape(p, r, 1);
                }
                else if(_shape == BOX_SHAPE){
                    _collisionShape = new btBoxShape(btVector3(_mesh->getBoundingBox().getMax().x, _mesh->getBoundingBox().getMax().y, _mesh->getBoundingBox().getMax().z));   
                }            
               _collisionShape->setLocalScaling(btVector3(_scale.x, _scale.y, _scale.z));
               
                btVector3 inertia(0,0,0);
                _collisionShape->calculateLocalInertia(_mass, inertia);               
               _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(_mass, _parent, _collisionShape, inertia));
               _dynamicsWorld->addRigidBody(_rigidBody);
           }
           
           _needsRebuild = false;            
        }
    }

    void Prop::_entityAttached(){
        btVector3 inertia(0,0,0);
        _collisionShape->calculateLocalInertia(_mass, inertia);
        _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(_mass, _parent, _collisionShape, inertia));
        _rigidBody->setActivationState(DISABLE_DEACTIVATION);

        IObject* next = _parent;
        while(next != nullptr){
            if(next->getType().compare("scene") == 0){
                Scene* scene = static_cast<Scene*>(next);
                setDynamicsWorld(scene->getDynamicsWorld());
                break;
            }
            next = next->getParent();
        }
    }

    void Prop::setTransform(glm::mat4 transform){
        _transform = transform;
        _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_transform));
        _mesh->setTransform(_transform);
    }

    void Prop::setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld){
        if(_rigidBody != nullptr){
            _dynamicsWorld = dynamicsWorld;
            dynamicsWorld->addRigidBody(_rigidBody);
        }
        else
            printf("Warning: Prop::setDynamicsWorld(): entity not attached to SceneNode\n");
    }
    
    void Prop::setScale(glm::vec3 scale){
        _scale = scale;
        _needsRebuild = true;
        
        //_mesh->setTransform(glm::scale(_scale)/*_mesh->getTransform()*/); //TODO: separate scale matrix?
        //_collisionShape->setLocalScaling(btVector3(_scale.x, _scale.y, _scale.z));
    }          

}
