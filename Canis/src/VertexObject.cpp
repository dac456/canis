#include "VertexObject.h"
#include "MaterialManager.h"
#include "Material.h"
#include "GlShader.h"
#include "Texture.h"
#include "AxisAlignedBox.h"

namespace Canis
{

    VertexObject::VertexObject(Vertex vertices[], size_t numVertices, GLenum drawMode, size_t materialId){
        GLenum err = glGetError();
        //_aabb = new AxisAlignedBox();
        _material = MaterialManager::getSingleton().getMaterial(materialId);
        _lightmap = nullptr;
        _isIndexed = false;

        Vertex* v = new Vertex[numVertices];
        //GLubyte* idx = new GLubyte[numIndices];

        for(size_t i=0; i<numVertices; i++){
            v[i] = vertices[i];
            //fprintf(stdout, "%i: %f %f %f\n", i, v[i].vertex[0], v[i].vertex[1], v[i].vertex[2]);

            _aabb.addPoint(vertices[i].vertex);
        }
        //for(size_t i=0; i<numIndices; i++)
        //  idx[i] = indices[i];

        _vertices = v;
        _numVertices = numVertices;
        //_indices = idx;
        //_numIndices = numIndices;
        _drawMode = drawMode;

        _transform = glm::mat4(1.0);

        glGenVertexArrays(1, &_arrayId);
        glBindVertexArray(_arrayId);

        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, 0, sizeof(vertices[0]), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].vertex));
        glVertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(vertices[0]), (GLvoid*)(sizeof(vertices[0].vertex)+sizeof(vertices[0].normal)));
        glVertexAttribPointer(3, 2, GL_FLOAT, 0, sizeof(vertices[0]), (GLvoid*)(sizeof(vertices[0].vertex)+sizeof(vertices[0].normal)+sizeof(vertices[0].texCoord)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        /*glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*numIndices, &idx[0], GL_STATIC_DRAW);*/

        err = glGetError();
        //if(err != GL_NO_ERROR)
        //  fprintf(stderr, "%s\n", gluErrorString(err));

        //delete[] v;
        //delete idx;
    }

    VertexObject::VertexObject(Vertex vertices[], size_t numVertices, unsigned int indices[], size_t numIndices, GLenum drawMode, size_t materialId){
        GLenum err = glGetError();
        //_aabb = new AxisAlignedBox();
        _material = MaterialManager::getSingleton().getMaterial(materialId);
        _lightmap = nullptr;
        _isIndexed = true;

        Vertex* v = new Vertex[numVertices];
        //unsigned int* idx = new unsigned int[numIndices];

        for(size_t i=0; i<numVertices; i++){
            v[i] = vertices[i];

            _aabb.addPoint(v[i].vertex);
        }
        //for(size_t i=0; i<numIndices; i++)
        //  idx[i] = indices[i];

        _vertices = v;
        _numVertices = numVertices;
        _indices = indices;
        _numIndices = numIndices;
        _drawMode = drawMode;

        _transform = glm::mat4(1.0);

        glGenVertexArrays(1, &_arrayId);
        glBindVertexArray(_arrayId);

        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, v, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, 0, sizeof(v[0]), 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(v[0]), (GLvoid*)sizeof(v[0].vertex));
        glVertexAttribPointer(2, 2, GL_FLOAT, 0, sizeof(v[0]), (GLvoid*)(sizeof(v[0].vertex)+sizeof(v[0].normal)));
        glVertexAttribPointer(3, 2, GL_FLOAT, 0, sizeof(v[0]), (GLvoid*)(sizeof(v[0].vertex)+sizeof(v[0].normal)+sizeof(v[0].texCoord)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glGenBuffers(1, &_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*numIndices, &indices[0], GL_STATIC_DRAW);
        
        glGenBuffers(1, &_transformBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _transformBuffer);
        for(size_t i=0; i<4; i++){
            glEnableVertexAttribArray(4 + i);
            glVertexAttribPointer(4 + i, 4, GL_FLOAT, 0, sizeof(GLfloat)*16, (const GLvoid*)(sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(4 + i, 1);
        }

        err = glGetError();
        //if(err != GL_NO_ERROR)
        //  fprintf(stderr, "%s\n", gluErrorString(err));

        //delete[] v;
        //delete[] idx;
    }

    VertexObject::~VertexObject(){
        delete[] _vertices;
        delete _lightmap;
        //delete _aabb;
        delete[] _indices;
    }

    void VertexObject::render(size_t numInstances, GLfloat* transformArray){
        glBindBuffer(GL_ARRAY_BUFFER, _transformBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*16*numInstances, transformArray, GL_DYNAMIC_DRAW);
        
        glBindVertexArray(_arrayId);
        if(_isIndexed)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

        if(!_isIndexed)
            glDrawArrays(_drawMode, 0, _numVertices);
        else
            glDrawElementsInstanced(_drawMode, _numIndices, GL_UNSIGNED_INT, NULL, numInstances);
        glDisable(GL_BLEND);       
    }

    void VertexObject::scale(Real s){
        _transform = glm::scale(glm::mat4(1.0f), glm::vec3(s, s, s));
    }

    void VertexObject::setLightmap(Texture* lightmap){
        _lightmap = lightmap;
    }

    Material* VertexObject::getMaterial(){
        return _material;
    }

    Texture* VertexObject::getLightmap(){
        return _lightmap;
    }

    glm::mat4 VertexObject::getTransform(){
        return _transform;
    }

    AxisAlignedBox VertexObject::getBoundingBox(){
        return _aabb;
    }

    Vertex* VertexObject::getVertices(){
        return _vertices;
    }

    size_t VertexObject::getNumVertices(){
        return _numVertices;
    }

    unsigned int* VertexObject::getIndices(){
        return _indices;
    }

    size_t VertexObject::getNumIndices(){
        return _numIndices;
    }

    bool VertexObject::isIndexed(){
        return _isIndexed;
    }

}
