#include <boost/filesystem.hpp>
#include "AssimpLoader.h"
#include "GlShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "Material.h"
#include "Texture.h"

using namespace boost;

namespace Canis
{

    AssimpLoader::AssimpLoader(std::string fileName){
        Assimp::Importer imp;
        _scene = imp.ReadFile(fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

        if(!_scene){
            _loaded = false;
            fprintf(stdout, "%s\n", imp.GetErrorString());
        }
        else
            load(fileName);
    }

    AssimpLoader::~AssimpLoader(){
        //delete _scene;
    }

    void AssimpLoader::load(std::string fileName){
        _data.resize(_scene->mNumMeshes);
        _indices.resize(_scene->mNumMeshes);

        for(size_t i=0; i<_scene->mNumMeshes; i++){
            aiMesh* mesh = _scene->mMeshes[i];

            _data[i].first.resize(_scene->mNumMeshes);
            _indices[i].resize(_scene->mNumMeshes);

            GroupData group;
            _data.push_back(group);
            ObjectData object;
            _data[i].first.push_back(object);

            std::vector< std::pair<unsigned int*, size_t> > groupIdx;
            _indices.push_back(groupIdx);
            std::pair<unsigned int*, size_t> objectIdx;
            _indices[i].push_back(objectIdx);

            unsigned int* indices = new unsigned int[mesh->mNumFaces*3];
            unsigned int faceIdx = 0;
            for(size_t j=0; j<mesh->mNumFaces; j++){
                aiFace face = mesh->mFaces[j];
                for(size_t k=0; k<face.mNumIndices; k++)
                    indices[faceIdx+k] = face.mIndices[k];
                faceIdx += 3;
            }

            _indices[i][i].first = indices;
            _indices[i][i].second = mesh->mNumFaces*3;
            //for(size_t j=0; j<_indices[i][i].second; j++)
            //  fprintf(stdout, "%i\n", _indices[i][i].first[j]);

            Vertex v;

            if(mesh->HasPositions()){
                for(size_t j=0; j<mesh->mNumVertices; j++){
                    v.vertex[0] = mesh->mVertices[j].x;
                    v.vertex[1] = mesh->mVertices[j].y;
                    v.vertex[2] = mesh->mVertices[j].z;
                    v.vertex[3] = 1.0f;

                    if(mesh->HasNormals()){
                        v.normal[0] = mesh->mNormals[j].x;
                        v.normal[1] = mesh->mNormals[j].y;
                        v.normal[2] = mesh->mNormals[j].z;
                        v.normal[3] = 1.0f;
                    }

                    if(mesh->HasTextureCoords(0)){
                        v.texCoord[0] = mesh->mTextureCoords[0][j].x;
                        v.texCoord[1] = mesh->mTextureCoords[0][j].y;
                    }

                    _data[i].first[i].first.push_back(v);
                }
            }

            aiMaterial* aiMat = _scene->mMaterials[mesh->mMaterialIndex];

            aiString matName;
            aiMat->Get(AI_MATKEY_NAME, matName);
            
            Material* mat = MaterialManager::getSingleton().getMaterial(std::string(matName.C_Str()));
            if(mat == nullptr){
                Technique t;
                Pass p;
                
                aiColor4D diffuse;
                aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

                p.diffuse.r = diffuse.r;
                p.diffuse.g = diffuse.g;
                p.diffuse.b = diffuse.b;
                p.diffuse.a = diffuse.a;
                p.blend = false;

                p.shader = GlShaderManager::getSingleton().getShader("basic");

                if(aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0){
                    aiString fileName;
                    aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
                    std::string fname = filesystem::path(fileName.C_Str()).stem().string();
                    
                    Texture* tex = TextureManager::getSingleton().getTexture(fname);
                    if(tex == nullptr){
                        tex = new Texture(fname, std::string(fileName.C_Str()), 0);
                        TextureManager::getSingleton().addTexture(tex);
                    }

                    p.textures.push_back(tex->getId());
                }

                t.passes.push_back(p);

                mat = new Material(std::string(matName.C_Str()), t);
                _data[i].second = MaterialManager::getSingleton().addMaterial(mat);
            }
            else{
                _data[i].second = mat->getId();
            }                
        }

        _loaded = true;
    }

    VertexData AssimpLoader::getData(){
        if(_loaded)
            return _data;
        else{
            VertexData ret;
            return ret;
        }
    }

    IndexData AssimpLoader::getIndices(){
        return _indices;
    }

}

