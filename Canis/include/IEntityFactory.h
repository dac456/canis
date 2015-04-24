#ifndef __ENTITYFACTORY_H
#define __ENTITYFACTORY_H

#include "Common.h"

namespace Canis
{

    class CSAPI IEntityFactory{
    public:
        IEntityFactory(){}
        virtual ~IEntityFactory(){}

        virtual IEntityPtr createEntity(std::string name, glm::mat4 transform = glm::mat4(1.0f), const StringMap& userParams = StringMap()) = 0;
    };

}

#endif
