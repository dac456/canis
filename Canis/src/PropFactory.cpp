#include "PropFactory.h"
#include "EntityManager.h"
#include "Prop.h"
#include "Mesh.h"
#include "AssimpLoader.h"

namespace Canis
{

    PropFactory::PropFactory(){
        EntityManager::getSingleton().addEntityFactory("Prop", this);
    }

    PropFactory::~PropFactory(){
    }

    IEntity* PropFactory::createEntity(std::string name, glm::mat4 transform, const StringMap& userParams){
        StringMap params = userParams; //TODO: should userParams be const?

        float mass = std::stof(params["mass"]);
        
        COLLISION_SHAPE shape = BOX_SHAPE;
        if(params.count("shape") == 1){
            shape = static_cast<COLLISION_SHAPE>(std::stoi(params["shape"]));
        }
        
        std::string mesh = params["mesh"];

        Prop* prop = new Prop(name, new Mesh(new AssimpLoader(mesh)), shape, mass);
        prop->setParam("mesh", mesh);
        
        return prop;
    }

}
