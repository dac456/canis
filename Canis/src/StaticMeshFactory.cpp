#include "StaticMeshFactory.h"
#include "StaticMesh.h"
#include "EntityManager.h"
#include "Mesh.h"
#include "AssimpLoader.h"

namespace Canis
{

    StaticMeshFactory::StaticMeshFactory(){
        EntityManager::getSingleton().addEntityFactory("Static Mesh", this);
    }

    IEntity* StaticMeshFactory::createEntity(std::string name, glm::mat4 transform, const StringMap& userParams){
        StringMap params = userParams;

        std::string mesh = params["mesh"];
        return new StaticMesh(name, new Mesh(new AssimpLoader(mesh)), transform);
    }


}
