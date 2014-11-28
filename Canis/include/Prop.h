#ifndef __PROP_H
#define __PROP_H

#include "Common.h"
#include "IEntity.h"

namespace Canis
{

    class CSAPI Prop : public IEntity{
    private:
        Mesh* _mesh;

        btCollisionShape* _collisionShape;
        btRigidBody* _rigidBody;
        float _mass;

    public:
        Prop(std::string name, Mesh* mesh, COLLISION_SHAPE shape, float mass);
        ~Prop();

        void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void reset();

        void setTransform(glm::mat4 transform);
        void setDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);

    private:
        void _entityAttached();
    };

}

#endif
