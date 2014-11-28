#ifndef __SKYBOX_H
#define __SKYBOX_H

#include "Common.h"

namespace Canis
{

    class CSAPI SkyBox{
    private:
        std::vector<VertexObject*> _vertexObjects;
        std::vector<Material*> _materials;
        Mesh* _mesh;

    public:
        SkyBox(std::string materialBaseName);

        void render();
    };

}

#endif
