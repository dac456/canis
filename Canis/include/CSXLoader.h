#ifndef __CSXLOADER_H
#define __CSXLOADER_H

#include "Common.h"
#include "IXmlLoader.h"
#include "VertexObject.h"

namespace Canis
{

    struct CSXBrush{
        std::vector<float> pos;
        std::vector<float> rot;
    };

    struct CSXFace{
        std::vector<float> normal;
        std::vector<float> texCoords;
    };

    class CSAPI CSXLoader : public IXmlLoader{
    private:
        VertexData _vertices;
        std::vector<glm::vec3> _currentVerts;
        glm::mat4 _currentTransform;
        CSXBrush _tempBrush;
        CSXFace _tempFace;


        int _currentGroup;
        int _currentFace;

    public:
        CSXLoader(std::string fileName);
        ~CSXLoader();

        VertexData getData();
        glm::mat4 getTransform();

        void smoothNormals();

    private:
        void stepAttributes(TiXmlElement* element) /*override*/;

        void optimize();
    };

}

#endif
