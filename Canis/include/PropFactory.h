#ifndef __PROPFACTORY_H
#define __PROPFACTORY_H

#include "Common.h"
#include "IEntityFactory.h"

namespace Canis
{

    class CSAPI PropFactory : public IEntityFactory{
    public:
        PropFactory();
        ~PropFactory();

        IEntityPtr createEntity(std::string name, glm::mat4 transform = glm::mat4(1.0f), const StringMap& userParams = StringMap());
    };

}

#endif
