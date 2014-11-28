#ifndef __PROGRAMLOADER_H
#define __PROGRAMLOADER_H

#include "Common.h"
#include "IXmlLoader.h"

namespace Canis
{
    typedef std::pair<std::string, std::pair<std::string, std::string>> ProgramLoaderData;

    class CSAPI ProgramLoader : public IXmlLoader{
    private:
        std::string _name;
        std::string _language;
        std::string _vertexShader;
        std::string _fragmentShader;

    public:
        ProgramLoader(std::string fileName);
        ~ProgramLoader(){}

        ProgramLoaderData getData();

    private:
        void stepAttributes(TiXmlElement* element) /*override*/;
    };

}

#endif
