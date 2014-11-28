#include <boost/filesystem.hpp>
#include "LW14Loader.h"
#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "Material.h"
#include "Texture.h"

using namespace boost;

namespace Canis
{

    LW14Loader::LW14Loader(std::string fileName){
        _scale = 32.0;
        if(fileName.find("skybox") != std::string::npos)
            _scale = 0.1;
            //_scale = 0.0001;

        //_currentBrush = -1;
        //_currentFace = -1;

        load(fileName);
    }

    LW14Loader::~LW14Loader(){
        for(size_t i=0; i<_materials.size(); i++)
            delete _materials[i];
        //for(size_t i=0; i<_brushes.size(); i++)
            //delete _brushes[i];
    }

    void LW14Loader::load(std::string fileName){
        std::ifstream fin(fileName, std::ios::in|std::ios::binary);

        //Header
        char* headerBlock = new char[sizeof(LW14Header)];
        fin.read(headerBlock, sizeof(LW14Header));
        LW14Header* header = reinterpret_cast<LW14Header*>(headerBlock);

        //Name Table
        fin.seekg(header->nameOffset, std::ios::beg);
        LW14NameTable* nameTable = new LW14NameTable;
        nameTable->name = new std::string[header->nameCount];
        for(int i=0; i<header->nameCount; i++){
            bool eos = false;

            while(!eos){
                char c = fin.get();
                if(c != '\0')
                    nameTable->name[i] += c;
                else
                    eos = true;
            }
        }

        //Object Table
        fin.seekg(header->objectOffset, std::ios::beg);
        for(int idx=0; idx<header->objectCount; idx++){
            char* objectBlock = new char[sizeof(LW14Object)];
            fin.read(objectBlock, sizeof(LW14Object));
            LW14Object* object = reinterpret_cast<LW14Object*>(objectBlock);

            std::string type = nameTable->name[object->objectClass-1];
            if(type.compare("group") == 0){
                char* groupBlock = new char[sizeof(LW14Group)];
                fin.read(groupBlock, sizeof(LW14Group));
                LW14Group* group = reinterpret_cast<LW14Group*>(groupBlock);

                _groups.push_back(group);
            }
            else if(type.compare("visgroup") == 0){
                fin.seekg(object->dataSize, std::ios::cur);
            }
            else if(type.compare("material") == 0){
                char* materialBlock = new char[sizeof(LW14Material)];
                fin.read(materialBlock, sizeof(LW14Material));
                LW14Material* material = reinterpret_cast<LW14Material*>(materialBlock);
                _materials.push_back(material);

                //fprintf(stdout, "Material: %s, %s\n", nameTable->name[material->groupName].c_str(), nameTable->name[material->objectName].c_str());

                if(2 & material->flags){
                    fin.seekg(sizeof(int), std::ios::cur);
                }
            }
            else if(type.compare("meshreference") == 0){
                char* meshReferenceBlock = new char[sizeof(LW14MeshRef)];
                fin.read(meshReferenceBlock, sizeof(LW14MeshRef));
                LW14MeshRef* meshRef = reinterpret_cast<LW14MeshRef*>(meshReferenceBlock);

                std::string groupName = nameTable->name[meshRef->groupName-1];
                std::string objName = nameTable->name[meshRef->objectName-1];
                fprintf(stdout, "Mesh Name: %s %s\n", groupName.c_str(), objName.c_str());

                //fin.seekg(object->dataSize, std::ios::cur);
            }
            else if(type.compare("lightmap") == 0){
                LW14LightMap lightmap;

                fin.read(reinterpret_cast<char*>(&lightmap.flags), sizeof(lightmap.flags));
                fin.read(reinterpret_cast<char*>(&lightmap.resolution), sizeof(lightmap.resolution));
                fin.read(reinterpret_cast<char*>(&lightmap.format), sizeof(lightmap.format));

                int size = 1 << lightmap.resolution;
                lightmap.pixels = new unsigned char[size*size*3];

                for(int i=0; i<(size*size); i++){
                    unsigned char* pixel = new unsigned char[3];
                    fin.read(reinterpret_cast<char*>(pixel), sizeof(unsigned char[3]));

                    lightmap.pixels[(i*3)] = pixel[0];
                    lightmap.pixels[(i*3)+1] = pixel[1];
                    lightmap.pixels[(i*3)+2] = pixel[2];
                }

                /*unsigned char* buffer = new unsigned char[res*res*3];
                for(size_t i=0; i<lightmap.pixels.size(); i++){
                    buffer[3*i] = lightmap.pixels[i][0];
                    buffer[(3*i)+1] = lightmap.pixels[i][1];
                    buffer[(3*i)+2] = lightmap.pixels[i][2];
                }*/

                std::string textureName = "LW14_LIGHTMAP_"+filesystem::path(fileName.c_str()).stem().string()+"_"+std::to_string((long long)idx+1);
                Texture* lightMap = new Texture(textureName, lightmap.pixels, size, 7);
                lightmap.texture = lightMap;
                lightmap.index = idx+1;
                _lightMaps.push_back(lightmap);

                //fin.seekg(object->dataSize, std::ios::cur);
            }
            else if(type.compare("brush") == 0){
                LW14Brush brush;

                //std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t>> > v;
                //std::pair<std::vector<Vertex>, size_t> o;
                //v.push_back(o);
                //_vertices.push_back(v);
                //_currentBrush++;

                fin.read(reinterpret_cast<char*>(&brush.flags), sizeof(brush.flags));
                fin.read(reinterpret_cast<char*>(&brush.keyCount), sizeof(brush.keyCount));
                if(brush.keyCount > 0){
                    brush.keys = new LW14Key[brush.keyCount];
                    for(long i=0; i<brush.keyCount; i++){
                        fin.read(reinterpret_cast<char*>(&brush.keys[i].keyName), sizeof(brush.keys[i].keyName));
                        fin.read(reinterpret_cast<char*>(&brush.keys[i].keyValue), sizeof(brush.keys[i].keyValue));
                    }
                }

                fin.read(reinterpret_cast<char*>(&brush.groupIndex), sizeof(brush.groupIndex));
                fin.read(reinterpret_cast<char*>(&brush.visgroupIndex), sizeof(brush.visgroupIndex));
                fin.read(reinterpret_cast<char*>(brush.colour), sizeof(brush.colour));
                fin.read(reinterpret_cast<char*>(&brush.vertexCount), sizeof(brush.vertexCount));

                brush.vertices = new glm::vec3[brush.vertexCount];
                for(int i=0; i<brush.vertexCount; i++){
                    float vertex[3];
                    fin.read(reinterpret_cast<char*>(vertex), sizeof(vertex));
                    brush.vertices[i] = glm::vec3(vertex[0]/_scale, vertex[1]/_scale, vertex[2]/_scale);
                    //fprintf(stdout, "V: %f, %f, %f\n", vertex[0]/32.0, vertex[1]/32.0, vertex[2]/32.0);
                }

                fin.read(reinterpret_cast<char*>(&brush.faceCount), sizeof(brush.faceCount));
                brush.faces = new LW14Face[brush.faceCount];
                for(int i=0; i<brush.faceCount; i++){
                    //std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > o;
                    //_vertices[_currentBrush].push_back(o);

                    fin.read(reinterpret_cast<char*>(&brush.faces[i].flags), sizeof(brush.faces[i].flags));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].planeEquation), sizeof(brush.faces[i].planeEquation));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].texturePosition), sizeof(brush.faces[i].texturePosition));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].textureScale), sizeof(brush.faces[i].textureScale));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].textureRotation), sizeof(brush.faces[i].textureRotation));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].uMap), sizeof(brush.faces[i].uMap));
                    fin.read(reinterpret_cast<char*>(brush.faces[i].vMap), sizeof(brush.faces[i].vMap));
                    fin.read(reinterpret_cast<char*>(&brush.faces[i].luxelSize), sizeof(brush.faces[i].luxelSize));
                    fin.read(reinterpret_cast<char*>(&brush.faces[i].smoothGroupIndex), sizeof(brush.faces[i].smoothGroupIndex));
                    fin.read(reinterpret_cast<char*>(&brush.faces[i].materialIndex), sizeof(brush.faces[i].materialIndex));
                    if(16 & brush.faces[i].flags)
                        fin.read(reinterpret_cast<char*>(&brush.faces[i].lightmapIndex), sizeof(brush.faces[i].lightmapIndex));
                    fin.read(reinterpret_cast<char*>(&brush.faces[i].indiceCount), sizeof(brush.faces[i].indiceCount));

                    //Materials
                    size_t mId = 0;
                    std::string matName = nameTable->name[_materials[brush.faces[i].materialIndex-1]->objectName-1];
                    Material* mat = MaterialManager::getSingleton().getMaterial(matName);
                    if(mat == nullptr){
                        /*mat = new Material(new MaterialLoader(matName+".material"));
                        if(!mat->getTechniques().empty())
                            mId = MaterialManager::getSingleton().addMaterial(mat);*/
                        mat = MaterialManager::getSingleton().getMaterial("system");
                    }
                    else
                        mId = mat->getId();

                    _currentGroup = mId;
                    if((mId+1) > _vertices.size()){
                        GroupData matGroup;
                        _vertices.resize(mId+1, matGroup);
                    }

                    ObjectData faceObj;
                    _vertices[_currentGroup].first.push_back(faceObj);
                    _currentFace = _vertices[_currentGroup].first.size()-1;

                    //mat->getTechniques()[0].passes[0].textures.push_back(_lightMaps[0].texture);

                    brush.faces[i].indices = new LW14Indice[brush.faces[i].indiceCount];
                    for(int j=0; j<brush.faces[i].indiceCount; j++){
                        fin.read(reinterpret_cast<char*>(&brush.faces[i].indices[j].vertex), sizeof(brush.faces[i].indices[j].vertex));
                        fin.read(reinterpret_cast<char*>(brush.faces[i].indices[j].textureCoords), sizeof(brush.faces[i].indices[j].textureCoords));
                        if(16 & brush.faces[i].flags)
                            fin.read(reinterpret_cast<char*>(brush.faces[i].indices[j].lightmapCoords), sizeof(brush.faces[i].indices[j].lightmapCoords));

                        Vertex vertex;
                        vertex.vertex[0] = brush.vertices[brush.faces[i].indices[j].vertex].x;
                        vertex.vertex[1] = brush.vertices[brush.faces[i].indices[j].vertex].y;
                        vertex.vertex[2] = brush.vertices[brush.faces[i].indices[j].vertex].z;
                        vertex.vertex[3] = 1.0;
                        vertex.normal[0] = brush.faces[i].planeEquation[0];
                        vertex.normal[1] = brush.faces[i].planeEquation[1];
                        vertex.normal[2] = brush.faces[i].planeEquation[2];
                        vertex.normal[3] = 1.0;
                        vertex.texCoord[0] = brush.faces[i].indices[j].textureCoords[0];
                        vertex.texCoord[1] = -brush.faces[i].indices[j].textureCoords[1];
                        vertex.lightMapCoord[0] = brush.faces[i].indices[j].lightmapCoords[0];
                        vertex.lightMapCoord[1] = brush.faces[i].indices[j].lightmapCoords[1];
                        _vertices[_currentGroup].first[_currentFace].first.push_back(vertex);
                        _vertices[_currentGroup].second = mId;
                        _vertices[_currentGroup].first[_currentFace].second = brush.faces[i].lightmapIndex;
                    }
                }
                _brushes.push_back(&brush);
            }
            else{
                fin.seekg(object->dataSize, std::ios::cur);
            }
        }
        fin.close();
        _isLoaded = true;
    }

    VertexData LW14Loader::getData(){
        if(_isLoaded)
            return _vertices;
        else{
            VertexData ret;
            return ret;
        }
    }

    Texture* LW14Loader::getLightmap(size_t index){
        for(size_t i=0; i<_lightMaps.size(); i++){
            if(_lightMaps[i].index == index)
                return _lightMaps[i].texture;
        }

        return nullptr;
    }

}
