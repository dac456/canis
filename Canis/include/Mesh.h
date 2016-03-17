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
        std::string _name;
        std::vector<MeshGroup> _groups;
        std::vector< std::pair<RenderablePtr,RenderableHandle> > _renderables;
        glm::mat4 _transform;

        AxisAlignedBox _aabb;
        Material* _overrideMaterial;
        
        bool _visible;

    public:
        Mesh(){}
        Mesh(DXSLoader* dxs);
        Mesh(LW14Loader* lw14);
        Mesh(std::string name, AssimpLoader* ai);
        Mesh(CSXLoader* csx);
        ~Mesh();

        void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<LightPtr> lights);
        void enqueue();
        
        RenderableList toRenderable();
        
        std::string getName();
        
        void setMaterialOverride(std::string name);
        void unsetMaterialOverride();

        void setTransform(glm::mat4 transform);
        glm::mat4 getTransform();

        AxisAlignedBox getBoundingBox();

        std::vector<MeshGroup> getMeshGroups();
    };

}

#endif
