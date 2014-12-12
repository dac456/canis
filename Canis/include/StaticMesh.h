#ifndef __STATICMESH_H
#define __STATICMESH_H

#include "Common.h"
#include "IEntity.h"

namespace Canis
{

    class CSAPI StaticMesh : public IEntity{
    private:
        Mesh* _mesh;

        //--Bullet--//
        btCollisionShape* _collisionShape;
        btRigidBody* _rigidBody;
        btTriangleMesh* _triangleMesh;
        btCompoundShape* _compoundShape;
        btDiscreteDynamicsWorld* _dynamicsWorld;

    public:
        StaticMesh(std::string name, Mesh* mesh, glm::mat4 transform = glm::mat4(1.0f));
        ~StaticMesh();

        void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

        void setTransform(glm::mat4 transform);
        void setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
        
        void scale(glm::vec3 scale);

    private:
        void _entityAttached();
    };

}

#endif
