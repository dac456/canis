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
        SceneWriter(ScenePtr scene);
        ~SceneWriter();
        
        void save(std::string file);
        
    private:
        void _writeNode(TiXmlElement* parent, SceneNodePtr node);
        void _writeEntity(TiXmlElement* parent, IEntityPtr ent);
        void _writeLight(TiXmlElement* parent, LightPtr light);
    };
    
}

#endif
