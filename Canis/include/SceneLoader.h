#ifndef __SCENELOADER_H
#define __SCENELOADER_H

#include "Common.h"
#include "TinyXML/tinyxml.h"

namespace Canis
{
    
    class CSAPI SceneLoader{
    private:
        ScenePtr _scene;
        IObjectPtr _lastObj;
        std::string _lastElem;
        
    public:
        SceneLoader(std::string file);
        ~SceneLoader();
        
        ScenePtr getScene();
        
    private:
        void _load(std::string file);
        void _loadElement(TiXmlNode* parent);
        void _loadNode(TiXmlNode* parent, IObjectPtr parentObj);
        void _loadEntity(TiXmlNode* parent, IObjectPtr parentObj);
        void _loadLight(TiXmlNode* parent, IObjectPtr parentObj);
        
        void _loadMat4(TiXmlNode* data, glm::mat4& mat);
        void _loadVec3(TiXmlNode* data, glm::vec3& vec);
    };
    
}

#endif
