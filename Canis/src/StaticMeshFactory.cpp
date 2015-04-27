#include "StaticMeshFactory.h"
#include "StaticMesh.h"
#include "EntityManager.h"
#include "Mesh.h"
#include "MeshManager.h"
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

        std::string meshPath = params["mesh"];
        filesystem::path p(meshPath);
        
        MeshPtr mesh = MeshManager::getSingleton().getMesh(p.filename().string());
        if(!mesh){
            mesh = std::make_shared<Mesh>(p.filename().string(), new AssimpLoader(p));
            MeshManager::getSingleton().addMesh(mesh);
        }        
        
        if(p.extension().string() == ".3dw"){
            return std::make_shared<StaticMesh>(name, std::make_shared<Mesh>(new LW14Loader(meshPath)), transform);
        }
        else{
            std::shared_ptr<StaticMesh> sm = std::make_shared<StaticMesh>(name, mesh, transform);
            sm->setParam("mesh", meshPath);
            
            return std::static_pointer_cast<IEntity>(sm);
        }
    }


}
