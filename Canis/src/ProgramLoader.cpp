#include "ProgramLoader.h"

namespace Canis
{

    ProgramLoader::ProgramLoader(std::string fileName) : IXmlLoader(){
        load("./Media/Shaders/"+fileName);
    }

    ProgramLoaderData ProgramLoader::getData(){
        ProgramLoaderData out;
        out.first = _name;
        out.second.first = _vertexShader;
        out.second.second = _fragmentShader;

        return out;
    }

    void ProgramLoader::stepAttributes(TiXmlElement* element){
        TiXmlAttribute* attrib = element->FirstAttribute();

        if(strcmp(element->Value(), "program") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "name") == 0)
                    _name = attrib->Value();
                if(strcmp(attrib->Name(), "language") == 0)
                    _language = attrib->Value();
                attrib = attrib->Next();
            }
        }
        if(strcmp(element->Value(), "vertex") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "file") == 0)
                    _vertexShader = attrib->Value();
                attrib = attrib->Next();
            }
        }
        if(strcmp(element->Value(), "fragment") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "file") == 0)
                    _fragmentShader = attrib->Value();
                attrib = attrib->Next();
            }
        }
    }

}
