#include "VertexObject.h"
#include "MaterialManager.h"
#include "Material.h"
#include "RenderManager.h"
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

    void VertexObject::render(){
        //
        //

        glm::vec4* frustum = RenderManager::getSingleton().getFrustum();
        int k = 0;
        /*for(size_t i=0; i<_numVertices; i++){
            if((_vertices[i].vertex[0]*frustum[0].x + _vertices[i].vertex[1]*frustum[0].y + _vertices[i].vertex[2]*frustum[0].z + frustum[0].w <= 0) ||
                (_vertices[i].vertex[0]*frustum[1].x + _vertices[i].vertex[1]*frustum[1].y + _vertices[i].vertex[2]*frustum[1].z + frustum[1].w <= 0) ||
                (_vertices[i].vertex[0]*frustum[2].x + _vertices[i].vertex[1]*frustum[2].y + _vertices[i].vertex[2]*frustum[2].z + frustum[2].w <= 0) ||
                (_vertices[i].vertex[0]*frustum[3].x + _vertices[i].vertex[1]*frustum[3].y + _vertices[i].vertex[2]*frustum[3].z + frustum[3].w <= 0) ||
                (_vertices[i].vertex[0]*frustum[4].x + _vertices[i].vertex[1]*frustum[4].y + _vertices[i].vertex[2]*frustum[4].z + frustum[4].w <= 0) ||
                (_vertices[i].vertex[0]*frustum[5].x + _vertices[i].vertex[1]*frustum[5].y + _vertices[i].vertex[2]*frustum[5].z + frustum[5].w <= 0))
                k++;
        }*/

        //_center = RenderManager::getSingleton().getProjectionMatrix()*RenderManager::getSingleton().getViewMatrix()*_center;
        for(size_t i=0; i<6; i++){
            if(_aabb.getCenter().x*frustum[i].x + _aabb.getCenter().y*frustum[i].y + _aabb.getCenter().z*frustum[i].z + frustum[i].w <= 0)
                k++;
        }

        //if(k == 0/*k != _numVertices*/){
            //fprintf(stdout, "Drawing VO\n");

            //glBlendFunc(GL_ONE, GL_ONE);
            glBindVertexArray(_arrayId);
            if(_isIndexed)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

            /*if(_material != nullptr){
                Technique t = _material->getTechniques()[0];
                for(size_t i=0; i<t.passes.size(); i++){
                    GLuint shd = t.passes[i].shader->getProgramId();
                    t.passes[i].shader->setUniformMat4f("cs_ViewMatrix", RenderManager::getSingleton().getViewMatrix());
                    t.passes[i].shader->setUniformMat4f("cs_ProjectionMatrix", RenderManager::getSingleton().getProjectionMatrix());
                    t.passes[i].shader->setUniformMat4f("cs_ModelMatrix", _transform);
                    t.passes[i].shader->setUniformMat4f("cs_NormalMatrix", glm::transpose(glm::inverse(glm::mat4(_transform*RenderManager::getSingleton().getViewMatrix()))));
                    t.passes[i].shader->use();
                    for(size_t j=0; j<t.passes[i].textures.size(); j++)
                        t.passes[i].textures[j]->use(shd);

                    if(_lightmap != nullptr)
                        _lightmap->use(shd);*/

                    //fprintf(stdout, "%i\n", t.passes[i].textures.size());

                    /*if(i == 0){
                        glBlendFunc(GL_ONE, GL_ZERO);
                        glDepthMask(GL_TRUE);
                        glDepthFunc(GL_LEQUAL);
                    }
                    else{
                        t.passes[i].shader->setUniformMat4f("modelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(2.0, 2.0, 2.0)));
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_ONE, GL_ONE);
                        glDepthMask(GL_FALSE);
                    }*/
                    if(!_isIndexed)
                        glDrawArrays(_drawMode, 0, _numVertices);
                    else
                        glDrawElements(_drawMode, _numIndices, GL_UNSIGNED_INT, NULL);
                    glDisable(GL_BLEND);
                //}
            //}
        //}
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
