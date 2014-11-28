#ifndef __MESH_H
#define __MESH_H

#include "Common.h"

namespace Canis
{
    struct MeshGroup{
        size_t id;
        size_t materialId;
        std::vector<VertexObject*> vertexObjects;
        AxisAlignedBox boundingBox;
    };

    class CSAPI Mesh{
    private:
        size_t _id;
        std::vector<MeshGroup> _groups;
        glm::mat4 _transform;

        AxisAlignedBox _aabb;

    public:
        Mesh(){}
        Mesh(DXSLoader* dxs);
        Mesh(LW14Loader* lw14);
        Mesh(AssimpLoader* ai);
        Mesh(CSXLoader* csx);
        ~Mesh();

        void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<Light*> lights);

        void setTransform(glm::mat4 transform);
        glm::mat4 getTransform();

        AxisAlignedBox getBoundingBox();

        std::vector<MeshGroup> getMeshGroups();
    };

}

#endif
