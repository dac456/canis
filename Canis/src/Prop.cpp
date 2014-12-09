#include "Prop.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Engine.h"

namespace Canis
{

    Prop::Prop(std::string name, Mesh* mesh, COLLISION_SHAPE shape, float mass)
        : IEntity("prop", name, glm::mat4(1.0f), true){
            _mesh = mesh;
            _mesh->setTransform(glm::mat4(1.0f));
            _mass = mass;
            
            _paramTypes["name"] = PARAM_STRING;
            _paramTypes["mesh"] = PARAM_PATH;
            _paramTypes["shape"] = PARAM_SHAPE;
            _paramTypes["mass"] = PARAM_REAL;
            
            _params["name"] = name;
            
            std::stringstream ss;
            ss << mass;
            _params["mass"] = ss.str();

            if(shape == SPHERE_SHAPE)
                _collisionShape = new btSphereShape(_mesh->getBoundingBox().getRadius());
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

        if(_parent != nullptr){
            glm::mat4 absTransform = _parent->getTransform()*_transform;
            IObject* next = _parent->getParent();
            while(next != nullptr){
                absTransform = next->getTransform()*absTransform;
                next = next->getParent();
            }
            _mesh->setTransform(absTransform);
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(absTransform));
            //_rigidBody->setWorldTransform(_dynamicsTransform);
        }
        else{
            _mesh->setTransform(localTransform);
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
        //_mesh->setTransform(localTransform);
        
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
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_parent->getInitialTransform()));
            _rigidBody->clearForces();
            _rigidBody->setLinearVelocity(btVector3(0,0,0));
            _rigidBody->setAngularVelocity(btVector3(0,0,0));
            _rigidBody->setWorldTransform(_dynamicsTransform);
        }
    }

    void Prop::_entityAttached(){
        btVector3 inertia(0,0,0);
        _collisionShape->calculateLocalInertia(_mass, inertia);
        _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(_mass, _parent, _collisionShape, inertia));

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
        if(_rigidBody != nullptr)
            dynamicsWorld->addRigidBody(_rigidBody);
        else
            printf("Warning: Prop::setDynamicsWorld(): entity not attached to SceneNode\n");
    }

}
