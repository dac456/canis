#ifndef __STATICMESHFACTORY_H
#define __STATICMESHFACTORY_H

#include "Common.h"
#include "IEntityFactory.h"

namespace Canis
{

    class CSAPI StaticMeshFactory : public IEntityFactory{
    public:
        StaticMeshFactory();
        ~StaticMeshFactory(){}

        IEntity* createEntity(std::string name, glm::mat4 transform = glm::mat4(1.0f), const StringMap& userParams = StringMap());
    };

}

#endif
