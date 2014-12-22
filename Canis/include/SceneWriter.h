#ifndef __SCENEWRITER_H
#define __SCENEWRITER_H

#include "Common.h"
#include "TinyXML/tinyxml.h"

namespace Canis
{
    
    class CSAPI SceneWriter{
    private:
        TiXmlDocument _doc;
        
    public:
        SceneWriter(Scene* scene);
        ~SceneWriter();
        
        void save(std::string file);
        
    private:
        void _writeNode(TiXmlElement* parent, SceneNode* node);
        void _writeEntity(TiXmlElement* parent, IEntity* ent);
        void _writeLight(TiXmlElement* parent, Light* light);
    };
    
}

#endif
