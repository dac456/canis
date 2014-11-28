#ifndef __ENTITYDIALOGMANAGER_H
#define __ENTITYDIALOGMANAGER_H

#include <map>
#include <assert.h>
#include <Singleton.h>

#include "IEntityDialogFactory.h"

namespace SceneEditor
{

    class EntityDialogManager : public Canis::Singleton<EntityDialogManager>{
    private:
        std::map<std::string, IEntityDialogFactory*> _factories;

    public:
        EntityDialogManager();
        ~EntityDialogManager();

        void addEntityDialogFactory(std::string entityType, IEntityDialogFactory* factory);
        IEntityDialogFactory* getEntityDialogFactory(std::string entityType);
    };

}

#endif
