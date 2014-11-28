#include "StaticMesh.h"
#include "Mesh.h"
#include "VertexObject.h"
#include "Scene.h"
#include "SceneNode.h"

namespace Canis
{

    StaticMesh::StaticMesh(std::string name, Mesh* mesh, glm::mat4 transform)
        : IEntity("staticmesh", name, transform, true){
            _mesh = mesh;
            _mesh->setTransform(transform);
            printf("Entity %s\n", name.c_str());

            //--Bullet--//
            _triangleMesh = nullptr;
            _compoundShape = nullptr;

            std::vector<MeshGroup> groups = _mesh->getMeshGroups();
            for(size_t i=0; i<groups.size(); i++){

                std::vector<VertexObject*> objects = groups[i].vertexObjects;
                for(size_t j=0; j<objects.size(); j++){
                    VertexObject* obj = objects[j];

                    if(obj->isIndexed() && (obj->getNumIndices() >= 3)){
                        if(_triangleMesh == nullptr)
                            _triangleMesh = new btTriangleMesh();

                        for(size_t k=0; k<(obj->getNumIndices()-3); k+=3){
                            btVector3 A(obj->getVertices()[obj->getIndices()[k]].vertex[0], obj->getVertices()[obj->getIndices()[k]].vertex[1], obj->getVertices()[obj->getIndices()[k]].vertex[2]);
                            btVector3 B(obj->getVertices()[obj->getIndices()[k+1]].vertex[0], obj->getVertices()[obj->getIndices()[k+1]].vertex[1], obj->getVertices()[obj->getIndices()[k+1]].vertex[2]);
                            btVector3 C(obj->getVertices()[obj->getIndices()[k+2]].vertex[0], obj->getVertices()[obj->getIndices()[k+2]].vertex[1], obj->getVertices()[obj->getIndices()[k+2]].vertex[2]);
                            
                            _triangleMesh->addTriangle(A, B, C);
                        }
                    }
                    else{
                        //TODO: when tested with LW14 meshes, this doesn't work properly
                        if(_compoundShape == nullptr)
                            _compoundShape = new btCompoundShape();

                        btConvexHullShape* hull = new btConvexHullShape();
                        for(size_t k=0; k<obj->getNumVertices(); k++)
                            hull->addPoint(btVector3(obj->getVertices()[k].vertex[0], obj->getVertices()[k].vertex[1], obj->getVertices()[k].vertex[2]));
                        //btBoxShape* box = new btBoxShape(btVector3(obj->getBoundingBox().getMax().x-obj->getBoundingBox().getCenter().x, obj->getBoundingBox().getMax().y-obj->getBoundingBox().getCenter().y, obj->getBoundingBox().getMax().z-obj->getBoundingBox().getCenter().z));

                        btTransform local;
                        local.setIdentity();
                        _compoundShape->addChildShape(local, hull);
                    }

                }
            }
            
            if(_triangleMesh != nullptr)
                _collisionShape = new btBvhTriangleMeshShape(_triangleMesh, true);
            else if(_compoundShape != nullptr)
                _collisionShape = _compoundShape;

            _rigidBody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0, this, _collisionShape, btVector3(0, 0, 0)));
    }

    StaticMesh::~StaticMesh(){
        delete _mesh;

        delete _rigidBody;
        delete _collisionShape;

        if(_triangleMesh != nullptr)
            delete _triangleMesh;
    }

    void StaticMesh::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
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
            _rigidBody->setWorldTransform(_dynamicsTransform);
        }
        else{
            _mesh->setTransform(localTransform);
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(localTransform));
            _rigidBody->setWorldTransform(_dynamicsTransform);
        }

        std::vector<Light*> lights;
        SceneNode* parentNode = getParentNode();
        if(parentNode != nullptr){
            Scene* parentScene = parentNode->getParentScene();
            if(parentScene != nullptr){
                lights = parentScene->getLightsClosestToPoint(this->getAbsoluteTransform()[3]);
            }
        }
        _mesh->render(projectionMatrix, viewMatrix, lights);
        //_mesh->setTransform(localTransform);
        //_dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(localTransform));
        //_rigidBody->setWorldTransform(_dynamicsTransform);
    }

    void StaticMesh::_entityAttached(){
        IObject* next = _parent;
        while(next != nullptr){
            if(next->getType() == "scene"){
                Scene* scene = static_cast<Scene*>(next);
                setDynamicsWorld(scene->getDynamicsWorld());
                break;
            }
            next = next->getParent();
        }
    }

    void StaticMesh::setTransform(glm::mat4 transform){
        _transform = transform;
        //_dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_transform));
        _mesh->setTransform(_transform);
    }

    void StaticMesh::setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld){
        if(_rigidBody != nullptr)
            dynamicsWorld->addRigidBody(_rigidBody);
        else
            printf("Warning: StaticMesh::setDynamicsWorld(): entity not attached to SceneNode\n");
    }

}
