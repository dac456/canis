#ifndef __IXMLLOADER_H
#define __IXMLLOADER_H

#include "Common.h"
#include "TinyXML/tinyxml.h"

namespace Canis
{

    class CSAPI IXmlLoader{
    private:
        bool _isLoaded;

    public:
        IXmlLoader(){
            _isLoaded = false;
        }
        virtual ~IXmlLoader(){}

        virtual void load(std::string fileName){
            if(!_isLoaded){
                TiXmlDocument doc(fileName.c_str());
                if(doc.LoadFile()){
                    step(&doc);
                    _isLoaded = true;
                }
            }
        }

        virtual bool isLoaded(){
            return _isLoaded;
        }

    private:
        virtual void step(TiXmlNode* parent){
            TiXmlNode* child;

            switch(parent->Type()){
            case TiXmlNode::TINYXML_DOCUMENT:
                break;

            case TiXmlNode::TINYXML_ELEMENT:
                stepAttributes(parent->ToElement());
                break;

            default:
                break;

            }

            for(child=parent->FirstChild(); child != 0; child = child->NextSibling())
                step(child);
        }

    protected:
        virtual void stepAttributes(TiXmlElement* element) = 0;
    };

}

#endif
