#include "EntityDialogManager.h"
#include "PropDialogFactory.h"

namespace SceneEditor
{

    EntityDialogManager::EntityDialogManager(){
        PropDialogFactory* propDialog = new PropDialogFactory();
        addEntityDialogFactory(propDialog->getEntityName().toStdString(), propDialog);
    }

    EntityDialogManager::~EntityDialogManager(){
        std::map<std::string, IEntityDialogFactory*>::iterator it;
        for(it = _factories.begin(); it != _factories.end(); it++){
            delete (*it).second;
        }
    }

    void EntityDialogManager::addEntityDialogFactory(std::string entityType, IEntityDialogFactory* factory){
        _factories[entityType] = factory;
    }

    IEntityDialogFactory* EntityDialogManager::getEntityDialogFactory(std::string entityType){
        return _factories[entityType];
    }

}
