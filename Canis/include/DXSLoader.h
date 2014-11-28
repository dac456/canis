#ifndef __DXSLOADER_H
#define __DXSLOADER_H

#include "Common.h"
#include "VertexObject.h"
#include "TinyXML/tinyxml.h"

namespace Canis
{

    struct DXSVertex{
        size_t id;
        glm::vec3 p;
    };

    struct DXSPrimitive{
        size_t id;
        std::vector<DXSVertex> v;
    };

    struct DXSMaterial{
        size_t id;
        std::string name;
        std::string texture;
    };

    class CSAPI DXSLoader{
    private:
        //primitive[i].vertexObject[j].(vertices[], material)
        VertexData _vertices;

        std::vector<DXSPrimitive> _dxsPrimitives;
        std::vector<DXSMaterial> _dxsMaterials;
        size_t _highestVertex;
        size_t _currentGroup;
        size_t _currentFace;
        size_t _currentMaterial;

        bool _loadingVertList;
        bool _isLoaded;
        bool _hasNormals;

    public:
        DXSLoader();
        DXSLoader(std::string fileName);
        void load(std::string fileName);

        VertexData getData();

    private:
        void step(TiXmlNode* parent);
        void stepAttributes(TiXmlElement* element);
        void calculateNormals(bool averageNormals);
    };

}

#endif
