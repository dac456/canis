#ifndef __VERTEXOBJECT_H
#define __VERTEXOBJECT_H

#include "Common.h"

namespace Canis
{

    struct Vertex{
        Real vertex[4];
        Real normal[4];
        Real texCoord[2];
        Real lightMapCoord[2];
        size_t boneID;
        size_t vertID;
    };
    
    //Group->VertexObject->(VertexList, (Material ID, Lightmap))
    typedef std::pair< std::vector<Vertex>, size_t > ObjectData; //pair(vertices, lightmap id)
    typedef std::pair< std::vector<ObjectData>, size_t > GroupData; //pair(objects, material id)
    typedef std::vector<GroupData> VertexData;
    //typedef std::vector<std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > >> VertexData;
    //typedef std::vector< std::pair< std::vector<Vertex>, std::pair<size_t, size_t> > > VertexData; 
    //Group->VertexObject->(IndexList, numIndices)
    typedef std::vector<std::vector< std::pair<unsigned int*, size_t> > > IndexData;
    
    class CSAPI VertexObject{
    private:
        GLuint _arrayId;
        GLuint _vertexBuffer, _indexBuffer;
        GLuint _transformBuffer, _lightPositionBuffer, _lightColorBuffer, _lightRadiiBuffer;
        GLenum _drawMode;
        bool _isIndexed;

        AxisAlignedBox _aabb;
        Texture* _lightmap;

        glm::mat4 _transform;

        Vertex* _vertices;
        size_t _numVertices;
        unsigned int* _indices;
        size_t _numIndices;
        Material* _material;

    public:
        VertexObject(Vertex vertices[], size_t numVertices, GLenum drawMode, size_t materialId);
        VertexObject(Vertex vertices[], size_t numVertices, unsigned int indices[], size_t numIndices, GLenum drawMode, size_t materialId);
        ~VertexObject();

        void render(size_t numInstances = 1, GLfloat* transformArray = nullptr, GLfloat* lightPositionArray = nullptr);
        void scale(Real s);

        void setLightmap(Texture* lightmap);

        Material* getMaterial();
        Texture* getLightmap();

        glm::mat4 getTransform();
        AxisAlignedBox getBoundingBox();

        Vertex* getVertices();
        size_t getNumVertices();
        unsigned int* getIndices();
        size_t getNumIndices();

        bool isIndexed();
    };

}

#endif
