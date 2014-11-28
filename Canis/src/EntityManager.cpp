#include "EntityManager.h"
#include "PropFactory.h"
#include "StaticMeshFactory.h"

namespace Canis
{

    EntityManager::EntityManager(){
        new Canis::PropFactory();
        new Canis::StaticMeshFactory();
    }

    EntityManager::~EntityManager(){
        std::map<std::string, IEntityFactory*>::iterator it;
        for(it = _factories.begin(); it != _factories.end(); it++){
            delete (*it).second;
        }
    }

    void EntityManager::addEntityFactory(std::string entityType, IEntityFactory* factory){
        _factories[entityType] = factory;
    }

    IEntityFactory* EntityManager::getEntityFactory(std::string entityType){
        return _factories[entityType];
    }

    std::map<std::string, IEntityFactory*> EntityManager::getEntityFactories(){
        return _factories;
    }

}
