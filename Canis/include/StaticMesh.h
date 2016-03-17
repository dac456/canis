#ifndef __STATICMESH_H
#define __STATICMESH_H

#include "Common.h"
#include "IEntity.h"

namespace Canis
{

    class CSAPI StaticMesh : public IEntity{
    private:
        //MeshPtr _mesh;
        RenderableList _renderables;
        bool _needsRebuild;

        //--Bullet--//
        btCollisionShape* _collisionShape;
        btRigidBody* _rigidBody;
        btTriangleMesh* _triangleMesh;
        btCompoundShape* _compoundShape;
        btDiscreteDynamicsWorld* _dynamicsWorld;

    public:
        StaticMesh(std::string name, MeshPtr mesh, glm::mat4 transform = glm::mat4(1.0f));
        ~StaticMesh();

        void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void reset();

        void setTransform(glm::mat4 transform);
        void setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
        
        void setScale(glm::vec3 scale);

    private:
        void _entityAttached();
        void _rebuildCollisionMesh();
    };

}

#endif
