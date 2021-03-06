#include "Mesh.h"
#include "VertexObject.h"
#include "CSXLoader.h"
#include "DXSLoader.h"
#include "LW14Loader.h"
#include "AssimpLoader.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "AxisAlignedBox.h"
#include "Material.h"
#include "GlShader.h"
#include "Texture.h"
#include "Light.h"

namespace Canis
{

    Mesh::Mesh(DXSLoader* dxs){
        VertexData primitives = dxs->getData();
        for(size_t i=0; i<primitives.size(); i++){
            MeshGroup prim;
            prim.id = i;
            GroupData group = primitives[i];
            for(size_t j=0; j<group.first.size(); j++){
                ObjectData objVerts = group.first[j];

                Vertex* objVertsA = new Vertex[objVerts.first.size()];
                for(size_t k=0; k<objVerts.first.size(); k++)
                    objVertsA[k] = objVerts.first[k];

                VertexObject* vertexObject = new VertexObject(objVertsA, objVerts.first.size(), GL_TRIANGLE_FAN, group.second);
                prim.vertexObjects.push_back(vertexObject);

                prim.boundingBox.addAxisAlignedBox(vertexObject->getBoundingBox());
                prim.materialId = group.second;

                delete[] objVertsA;
            }
            _groups.push_back(prim);
        }
        delete dxs;
    }

    Mesh::Mesh(LW14Loader* lw14){
        if(!lw14->getData().empty()){
            VertexData primitives = lw14->getData();
            for(size_t i=0; i<primitives.size(); i++){
                MeshGroup prim;
                prim.id = i;
                GroupData group = primitives[i];
                for(size_t j=0; j<group.first.size(); j++){
                    ObjectData objVerts = group.first[j];

                    Vertex* objVertsA = new Vertex[objVerts.first.size()];
                    for(size_t k=0; k<objVerts.first.size(); k++)
                        objVertsA[k] = objVerts.first[k];

                    VertexObject* vertexObject = new VertexObject(objVertsA, objVerts.first.size(), GL_TRIANGLE_FAN, group.second);
                    vertexObject->setLightmap(lw14->getLightmap(objVerts.second));
                    prim.vertexObjects.push_back(vertexObject);

                    prim.boundingBox.addAxisAlignedBox(vertexObject->getBoundingBox());
                    prim.materialId = group.second;

                    delete[] objVertsA;
                }
                _groups.push_back(prim);
            }
            delete lw14;
        }
    }

    Mesh::Mesh(std::string name, AssimpLoader* ai){
        _name = name;
        _overrideMaterial = nullptr;
        
        if(!ai->getData().empty()){
            VertexData meshes = ai->getData();
            IndexData meshIndices = ai->getIndices();
            for(size_t i=0; i<meshes.size(); i++){
                MeshGroup mesh;
                mesh.id = i;
                GroupData group = meshes[i];
                std::vector< std::pair<unsigned int*, size_t> > objectIdx = meshIndices[i];
                for(size_t j=0; j<group.first.size(); j++){
                    ObjectData objVerts = group.first[j];
                    std::pair<unsigned int*, size_t> objIndices = objectIdx[j];

                    //TODO: there's lots of array alloc/dealloc and pointers being passed in this process. need to traverse everything and make sure there are no leaks.
                    Vertex* objVertsA = new Vertex[objVerts.first.size()];
                    for(size_t k=0; k<objVerts.first.size(); k++)
                        objVertsA[k] = objVerts.first[k];

                    VertexObject* vertexObject = new VertexObject(objVertsA, objVerts.first.size(), objIndices.first, objIndices.second, GL_TRIANGLES, group.second);
                    //vertexObject->setLightmap(lw14->getLightmap(objVerts.second.second));
                    mesh.vertexObjects.push_back(vertexObject);
                    mesh.materialId = group.second;

                    mesh.boundingBox.addAxisAlignedBox(vertexObject->getBoundingBox());

                    delete[] objVertsA;
                    //delete[] objIndices.first;
                }
                _groups.push_back(mesh);
                _aabb.addAxisAlignedBox(mesh.boundingBox);
            }
            delete ai;
        }
    }

    Mesh::Mesh(CSXLoader* csx){
        VertexData primitives = csx->getData();
        _transform = csx->getTransform();
        for(size_t i=0; i<primitives.size(); i++){
            MeshGroup prim;
            prim.id = i;
            GroupData group = primitives[i];
            for(size_t j=0; j<group.first.size(); j++){
                ObjectData objVerts = group.first[j];

                Vertex* objVertsA = new Vertex[objVerts.first.size()];
                for(size_t k=0; k<objVerts.first.size(); k++)
                    objVertsA[k] = objVerts.first[k];

                VertexObject* vertexObject = new VertexObject(objVertsA, objVerts.first.size(), GL_TRIANGLE_FAN, group.second);
                prim.vertexObjects.push_back(vertexObject);

                prim.boundingBox.addAxisAlignedBox(vertexObject->getBoundingBox());
                prim.materialId = group.second;

                delete[] objVertsA;
            }
            _groups.push_back(prim);
        }
        delete csx;
    }

    Mesh::~Mesh(){
        for(size_t i=0; i<_groups.size(); i++)
            for(size_t j=0; j<_groups[i].vertexObjects.size(); j++)
                delete _groups[i].vertexObjects[j];
    }

    void Mesh::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<LightPtr> lights){
        Texture* currentLightmap = nullptr;

        for(size_t i=0; i<_groups.size(); i++){
            if(_groups[i].vertexObjects.size()){
                //glm::vec4* frustum = RenderManager::getSingleton().getFrustum();
                int k=0;

                glm::vec4 absCenter = _transform*glm::vec4(_groups[i].boundingBox.getCenter(), 1.0);
                //std::pair<std::pair<glm::mat4, glm::mat4>, glm::vec4> lights = RenderManager::getSingleton().getLightsClosestToPoint(absCenter);

                glm::mat4 retPos = glm::mat4(0.0f);
                glm::mat4 retCol = glm::mat4(0.0f);
                glm::vec4 retRad = glm::vec4(0.0f);

                for(size_t j=0; j<lights.size(); j++){
                    retPos[j] = glm::vec4(lights[j]->getAbsolutePosition(), 1.0f);
                    retCol[j] = glm::vec4(lights[j]->getDiffuse(), 1.0f);
                    retRad[j] = lights[j]->getRadius();
                }

                /*int count = 0;
                for(int j=0; j<4; j++)
                    if(lights.first[j][3] == -1.0f)
                        lights.second[j] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);*/

                Material* mat;
                if(_overrideMaterial == nullptr){
                    mat = MaterialManager::getSingleton().getMaterial(_groups[i].materialId);
                }
                else{
                    mat = _overrideMaterial;
                }
                
                if(k == 0){
                    if(mat != nullptr){
                        Technique t = mat->getTechniques()[0];
                        for(size_t j=0; j<t.passes.size(); j++){
                            GLuint shd = t.passes[j].shader->getProgramId();
                            t.passes[j].shader->use();
                            t.passes[j].shader->setUniformMat4f("cs_ViewMatrix", viewMatrix);
                            t.passes[j].shader->setUniformMat4f("cs_ProjectionMatrix", projectionMatrix);
                            t.passes[j].shader->setUniformMat4f("cs_ModelMatrix", _transform);
                            t.passes[j].shader->setUniformMat3f("cs_NormalMatrix", glm::inverseTranspose(glm::mat3(viewMatrix)*glm::mat3(_transform)));
                            t.passes[j].shader->setUniformVec4f("cs_ViewVector", glm::inverse(viewMatrix)[2]);
                            t.passes[j].shader->setUniformMat4f("cs_LightPositions", retPos);
                            t.passes[j].shader->setUniformMat4f("cs_LightColors", retCol);
                            t.passes[j].shader->setUniformVec4f("cs_LightRadius", retRad);
                            t.passes[j].shader->setUniformVec4f("cs_DiffuseMaterial", t.passes[j].diffuse);

                            if(t.passes[j].textures.size() > 0){
                                t.passes[j].shader->setUniform1i("cs_UseTexture", true);
                                for(size_t k=0; k<t.passes[j].textures.size(); k++){
                                    Texture* tex = TextureManager::getSingleton().getTexture(t.passes[j].textures[k]);
                                    tex->use(shd);
                                }
                            }
                            else
                                t.passes[j].shader->setUniform1i("cs_UseTexture", false);

                            for(size_t k=0; k<_groups[i].vertexObjects.size(); k++){
                                if((_groups[i].vertexObjects[k]->getLightmap() != nullptr) && (currentLightmap != _groups[i].vertexObjects[k]->getLightmap())){
                                    t.passes[j].shader->setUniform1i("cs_UseLightmap", true);
                                    _groups[i].vertexObjects[k]->getLightmap()->use(shd);
                                }
                                else
                                    t.passes[j].shader->setUniform1i("cs_UseLightmap", false);

                                if(t.passes[j].blend){
                                    glEnable(GL_BLEND);
                                    glBlendFunc(t.passes[j].blendSrc, t.passes[j].blendDst);
                                }

                                _groups[i].vertexObjects[k]->render();

                                glDisable(GL_BLEND);
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::string Mesh::getName(){
        return _name;
    }
    
    void Mesh::setMaterialOverride(std::string name){
        _overrideMaterial = MaterialManager::getSingleton().getMaterial(name);
    }
    
    void Mesh::unsetMaterialOverride(){
        _overrideMaterial = nullptr;
    }

    void Mesh::setTransform(glm::mat4 transform){
        _transform = transform;
    }

    glm::mat4 Mesh::getTransform(){
        return _transform;
    }

    AxisAlignedBox Mesh::getBoundingBox(){
        return _aabb;
    }

    std::vector<MeshGroup> Mesh::getMeshGroups(){
        return _groups;
    }

}
