#ifndef __MATERIALLOADER_H
#define __MATERIALLOADER_H

#include "Common.h"
#include "IXmlLoader.h"

namespace Canis
{

    class CSAPI MaterialLoader : public IXmlLoader{
    private:
        std::string _name;
        std::vector<Technique> _techniques;

    public:
        MaterialLoader(std::string fileName);
        ~MaterialLoader();

        std::pair<std::string, std::vector<Technique>> getData();

    private:
        void stepAttributes(TiXmlElement* element) /*override*/;
    };

}

#endif
