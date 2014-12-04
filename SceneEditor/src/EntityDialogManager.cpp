#include "EntityDialogManager.h"
#include "PropDialogFactory.h"
#include "StaticMeshDialogFactory.h"

namespace SceneEditor
{

    EntityDialogManager::EntityDialogManager(){
        //PropDialogFactory* propDialog = new PropDialogFactory();
        addEntityDialogFactory(new PropDialogFactory());
        addEntityDialogFactory(new StaticMeshDialogFactory());
    }

    EntityDialogManager::~EntityDialogManager(){
        std::map<std::string, IEntityDialogFactory*>::iterator it;
        for(it = _factories.begin(); it != _factories.end(); it++){
            delete (*it).second;
        }
    }

    void EntityDialogManager::addEntityDialogFactory(IEntityDialogFactory* factory){
        if(factory != nullptr){
            _factories[factory->getEntityName().toStdString()] = factory;
        }
    }

    IEntityDialogFactory* EntityDialogManager::getEntityDialogFactory(std::string entityType){
        return _factories[entityType];
    }

}
