#include "PropFactory.h"
#include "EntityManager.h"
#include "Prop.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "AssimpLoader.h"

using namespace boost;

namespace Canis
{

    PropFactory::PropFactory(){
        EntityManager::getSingleton().addEntityFactory("Prop", this);
    }

    PropFactory::~PropFactory(){
    }

    IEntityPtr PropFactory::createEntity(std::string name, glm::mat4 transform, const StringMap& userParams){
        StringMap params = userParams; //TODO: should userParams be const?

        float mass = std::stof(params["mass"]);
        
        COLLISION_SHAPE shape = BOX_SHAPE;
        if(params.count("shape") == 1){
            shape = static_cast<COLLISION_SHAPE>(std::stoi(params["shape"]));
        }
        
        std::string meshPath = params["mesh"];
        filesystem::path p(meshPath);

        MeshPtr mesh = MeshManager::getSingleton().getMesh(p.filename().string());
        if(!mesh){
            mesh = std::make_shared<Mesh>(p.filename().string(), new AssimpLoader(p));
            MeshManager::getSingleton().addMesh(mesh);
        }
        
        std::shared_ptr<Prop> prop = std::make_shared<Prop>(name, mesh, shape, mass);
        prop->setParam("mesh", meshPath);
        
        return std::static_pointer_cast<IEntity>(prop);
    }

}
