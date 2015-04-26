#include "StaticMeshFactory.h"
#include "StaticMesh.h"
#include "EntityManager.h"
#include "Mesh.h"
#include "AssimpLoader.h"
#include "LW14Loader.h"

using namespace boost;

namespace Canis
{

    StaticMeshFactory::StaticMeshFactory(){
        EntityManager::getSingleton().addEntityFactory("Static Mesh", this);
    }

    IEntityPtr StaticMeshFactory::createEntity(std::string name, glm::mat4 transform, const StringMap& userParams){
        StringMap params = userParams;

        std::string mesh = params["mesh"];
        filesystem::path p(mesh);
        
        if(p.extension().string() == ".3dw"){
            return std::make_shared<StaticMesh>(name, new Mesh(new LW14Loader(mesh)), transform);
        }
        else{
            std::shared_ptr<StaticMesh> sm = std::make_shared<StaticMesh>(name, new Mesh(p.filename().string(), new AssimpLoader(mesh)), transform);
            sm->setParam("mesh", mesh);
            
            return std::static_pointer_cast<IEntity>(sm);
        }
    }


}
