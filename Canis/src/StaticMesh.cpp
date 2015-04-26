#include "StaticMesh.h"
#include "Mesh.h"
#include "VertexObject.h"
#include "Scene.h"
#include "SceneNode.h"

namespace Canis
{

    StaticMesh::StaticMesh(std::string name, Mesh* mesh, glm::mat4 transform)
        : IEntity("Static Mesh", name, transform, true){
            _mesh = mesh;
            _mesh->setTransform(transform);
            
            _paramTypes["name"] = PARAM_STRING;
            _paramTypes["mesh"] = PARAM_PATH;

            //--Bullet--//
            _triangleMesh = nullptr;
            _compoundShape = nullptr;

            std::vector<MeshGroup> groups = _mesh->getMeshGroups();
            for(size_t i=0; i<groups.size(); i++){

                std::vector<VertexObject*> objects = groups[i].vertexObjects;
                for(size_t j=0; j<objects.size(); j++){
                    VertexObject* obj = objects[j];

                    if(obj->isIndexed() && (obj->getNumIndices() >= 3)){
                        if(_triangleMesh == nullptr){
                            _triangleMesh = new btTriangleMesh();
                            //_triangleMesh->setScaling(btVector3(2.0f, 2.0f, 2.0f));
                        }

                        for(size_t k=0; k<(obj->getNumIndices()-3); k+=3){
                            btVector3 A(obj->getVertices()[obj->getIndices()[k]].vertex[0], obj->getVertices()[obj->getIndices()[k]].vertex[1], obj->getVertices()[obj->getIndices()[k]].vertex[2]);
                            btVector3 B(obj->getVertices()[obj->getIndices()[k+1]].vertex[0], obj->getVertices()[obj->getIndices()[k+1]].vertex[1], obj->getVertices()[obj->getIndices()[k+1]].vertex[2]);
                            btVector3 C(obj->getVertices()[obj->getIndices()[k+2]].vertex[0], obj->getVertices()[obj->getIndices()[k+2]].vertex[1], obj->getVertices()[obj->getIndices()[k+2]].vertex[2]);
                            
                            _triangleMesh->addTriangle(A, B, C);
                        }
                    }
                }
            }
            
            if(_triangleMesh != nullptr){
                _collisionShape = new btBvhTriangleMeshShape(_triangleMesh, true);
            }
            
            //btCollisionShape* scaledShape = new btScaledBvhTriangleMeshShape(static_cast<btBvhTriangleMeshShape*>(_collisionShape), btVector3(1.0f, 1.0f, 1.0f));
            //_rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, this, scaledShape, btVector3(0, 0, 0)));
    }

    StaticMesh::~StaticMesh(){
        delete _mesh;

        _dynamicsWorld->removeRigidBody(_rigidBody);
        delete _collisionShape;
        delete _rigidBody;

        if(_triangleMesh != nullptr)
            delete _triangleMesh;
    }

    void StaticMesh::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
        glm::mat4 localTransform = _transform;
        //glm::mat4 meshTransform = _mesh->getTransform();

        _mesh->setTransform(_motionState.getBulletWorldTransform());
        //_mesh->setTransform(absTransform*glm::scale(_scale));
        //_dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(absTransform));
        //_rigidBody->setWorldTransform(_dynamicsTransform);

        std::vector<LightPtr> lights;
        SceneNodePtr parentNode = getParentNode();
        if(parentNode != nullptr){
            ScenePtr parentScene = parentNode->getParentScene();
            if(parentScene != nullptr){
                lights = parentScene->getLightsClosestToPoint(this->getAbsoluteTransform()[3]);
            }
        }
        _mesh->render(projectionMatrix, viewMatrix, lights);
        //_mesh->setTransform(meshTransform);
        //_dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(localTransform));
        //_rigidBody->setWorldTransform(_dynamicsTransform);
    }
    
    void StaticMesh::reset(){
        if(_needsRebuild) {
            _rebuildCollisionMesh();
        }
    }    

    void StaticMesh::setTransform(glm::mat4 transform){
        _transform = transform;
    }

    void StaticMesh::setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld){
        if(_rigidBody != nullptr){
            dynamicsWorld->addRigidBody(_rigidBody);
            _dynamicsWorld = dynamicsWorld;
        }
        else
            printf("Warning: StaticMesh::setDynamicsWorld(): entity not attached to SceneNode\n");
    }
    
    void StaticMesh::setScale(glm::vec3 scale){
        _scale = scale;
        
        //_mesh->setTransform(glm::scale(_scale)/*_mesh->getTransform()*/);        
        
        _needsRebuild = true;
    }
    
    void StaticMesh::_entityAttached(){
        glm::mat4 absTrans = this->getAbsoluteTransform();
        _motionState.setBulletWorldTransform(absTrans);
        
        btCollisionShape* scaledShape = new btScaledBvhTriangleMeshShape(static_cast<btBvhTriangleMeshShape*>(_collisionShape), btVector3(1.0f, 1.0f, 1.0f));
        _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, &_motionState, scaledShape, btVector3(0, 0, 0)));        
        
        IObjectPtr next = _parent;
        while(next != nullptr){
            if(next->getType() == "scene"){
                ScenePtr scene = std::static_pointer_cast<Scene>(next);
                setDynamicsWorld(scene->getDynamicsWorld());
                break;
            }
            next = next->getParent();
        }
    }
    
    void StaticMesh::_rebuildCollisionMesh(){
       btCollisionShape* scaledShape = new btScaledBvhTriangleMeshShape(static_cast<btBvhTriangleMeshShape*>(_collisionShape), btVector3(_scale.x, _scale.y, _scale.z));
       
       if(_dynamicsWorld && _rigidBody){
           _dynamicsWorld->removeRigidBody(_rigidBody);
           delete _rigidBody->getCollisionShape();
           delete _rigidBody;
           _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, &_motionState, scaledShape, btVector3(0, 0, 0)));
           _dynamicsWorld->addRigidBody(_rigidBody);
       }
       
       _needsRebuild = false;
    }

}
