#ifndef __PROP_H
#define __PROP_H

#include "Common.h"
#include "IEntity.h"

namespace Canis
{

    class CSAPI Prop : public IEntity{
    private:
        MeshPtr _mesh;
        bool _needsRebuild;

        btCollisionShape* _collisionShape;
        btRigidBody* _rigidBody;
        btDiscreteDynamicsWorld* _dynamicsWorld;
        float _mass;
        COLLISION_SHAPE _shape;

    public:
        Prop(std::string name, MeshPtr mesh, COLLISION_SHAPE shape, float mass);
        ~Prop();

        void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void reset();

        void setTransform(glm::mat4 transform);
        void setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
        
        void setScale(glm::vec3 scale);

    private:
        void _entityAttached();
    };

}

#endif
