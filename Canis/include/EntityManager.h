#ifndef __ENTITYMANAGER_H
#define __ENTITYMANAGER_H

#include "Common.h"
#include "Singleton.h"
#include "IEntityFactory.h"

namespace Canis
{

    class CSAPI EntityManager : public Singleton<EntityManager>{
    private:
        std::map<std::string, IEntityFactory*> _factories;

    public:
        EntityManager();
        ~EntityManager();

        void addEntityFactory(std::string entityType, IEntityFactory* factory);

        IEntityFactory* getEntityFactory(std::string entityType);
        std::map<std::string, IEntityFactory*> getEntityFactories();
    };

}

#endif
