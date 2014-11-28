#include "CSXLoader.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MaterialLoader.h"

namespace Canis
{

    CSXLoader::CSXLoader(std::string fileName){
        load(fileName);
        //calculateNormals(true);

        fprintf(stdout, "Num Groups: %i\n", _vertices.size());
        for(size_t i=0; i<_vertices.size(); i++){
            fprintf(stdout, "\tObjects in Group %i: %i\n", i, _vertices[i].first.size());
        }

    }

    CSXLoader::~CSXLoader(){
    }

    void CSXLoader::stepAttributes(TiXmlElement* element){
        TiXmlAttribute* attrib = element->FirstAttribute();

        if(strcmp(element->Value(), "Brush") == 0){
            _currentVerts.clear();
            _currentTransform = glm::mat4(1.0f);
            _tempBrush.pos.clear();
            _tempBrush.rot.clear();

            //std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > > p;
            //_vertices.push_back(p);
            //_currentBrush = _vertices.size()-1;

            while(attrib){
                if(strcmp(attrib->Name(), "pos") == 0){
                    std::string pos = attrib->Value();
                    std::string temp;
                    std::stringstream ss(pos);

                    while(ss >> temp)
                        _tempBrush.pos.push_back(atof(temp.c_str()));
                }
                if(strcmp(attrib->Name(), "rot") == 0){
                    std::string rot = attrib->Value();
                    std::string temp;
                    std::stringstream ss(rot);

                    while(ss >> temp)
                        _tempBrush.rot.push_back(atof(temp.c_str()));
                }
                attrib = attrib->Next();
            }

            glm::vec3 translate(_tempBrush.pos[0], _tempBrush.pos[1], _tempBrush.pos[2]);
            glm::vec4 rotate(_tempBrush.rot[0], _tempBrush.rot[1], _tempBrush.rot[2], _tempBrush.rot[3]);
            _currentTransform = glm::translate(translate);
            //_currentTransform = glm::rotate(trans, -90.0f, glm::vec3(1,0,0));
        }
        if(strcmp(element->Value(), "Vertex") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "pos") == 0){
                    std::vector<float> fPos;
                    std::string pos = attrib->Value();
                    std::string temp;
                    std::stringstream ss(pos);

                    while(ss >> temp)
                        fPos.push_back(atof(temp.c_str()));

                    glm::vec3 vPos(fPos[0], fPos[1], fPos[2]);
                    _currentVerts.push_back(vPos);
                }
                attrib = attrib->Next();
            }
        }
        if(strcmp(element->Value(), "Face") == 0){
            _tempFace.normal.clear();
            _tempFace.texCoords.clear();

            while(attrib){
                if(strcmp(attrib->Name(), "plane") == 0){
                    std::string n = attrib->Value();
                    std::string temp;
                    std::stringstream ss(n);

                    while(ss >> temp)
                        _tempFace.normal.push_back(atof(temp.c_str()));
                }
                if(strcmp(attrib->Name(), "texgens") == 0){
                    std::string tex = attrib->Value();
                    std::string temp;
                    std::stringstream ss(tex);

                    while(ss >> temp)
                        _tempFace.texCoords.push_back(atof(temp.c_str()));
                }
                if(strcmp(attrib->Name(), "material") == 0){
                    std::string matName = attrib->Value();

                    Material* mat = MaterialManager::getSingleton().getMaterial(matName);
                    size_t id = 0;
                    if(mat == nullptr){
                        mat = new Material(new MaterialLoader(matName+".material"));
                        if(!mat->getTechniques().empty())
                            id = MaterialManager::getSingleton().addMaterial(mat);
                    }
                    else
                        id = mat->getId();


                    _currentGroup = id;
                    if((id+1) > _vertices.size()){
                        GroupData matGroup;
                        _vertices.resize(id+1, matGroup);
                    }
                        
                }
                attrib = attrib->Next();
            }

            ObjectData faceObj;
            _vertices[_currentGroup].first.push_back(faceObj);
            _currentFace = _vertices[_currentGroup].first.size()-1;

        }
        if(strcmp(element->Value(), "Indices") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "indices") == 0){
                    std::vector<int> iIdx;
                    std::string pos = attrib->Value();
                    std::string temp;
                    std::stringstream ss(pos);

                    while(ss >> temp)
                        iIdx.push_back(atoi(temp.c_str()));

                    for(size_t i=0; i<iIdx.size(); i++){
                        Vertex v;

                        glm::vec4 vertexPos(_currentVerts[iIdx[i]].x, _currentVerts[iIdx[i]].y, _currentVerts[iIdx[i]].z, 1.0f);
                        glm::vec4 transformedVertex = _currentTransform*vertexPos;

                        v.vertex[0] = transformedVertex.x;
                        v.vertex[1] = transformedVertex.y;
                        v.vertex[2] = transformedVertex.z;
                        v.vertex[3] = 1.0f;
                        v.normal[0] = _tempFace.normal[0];
                        v.normal[1] = _tempFace.normal[1];
                        v.normal[2] = _tempFace.normal[2];
                        v.normal[3] = 1.0f;
                        v.texCoord[0] = 1.0f;
                        v.texCoord[1] = 1.0f;
                        //v.texCoord[0] = _tempFace.texCoords[2*i];
                        //v.texCoord[1] = _tempFace.texCoords[(2*i)+1];

                        _vertices[_currentGroup].first[_currentFace].first.push_back(v);
                        _vertices[_currentGroup].second = _currentGroup;

                    }
                }
                attrib = attrib->Next();
            }
        }
    }

    VertexData CSXLoader::getData(){
        return _vertices;
    }

    glm::mat4 CSXLoader::getTransform(){
        return glm::rotate(glm::mat4(1.0), -90.0f, glm::vec3(1,0,0));
    }

    void CSXLoader::smoothNormals(){
        for(size_t i=0; i<_vertices.size(); i++){
            for(size_t j=0; j<_vertices[i].first.size(); j++){
                std::vector<Vertex> list;

                for(size_t k=0; k<_vertices[i].first[j].first.size(); k++){
                    list.push_back(_vertices[i].first[j].first[k]);
                }

                glm::vec4 n;
                for(size_t k=0; k<list.size(); k++){
                    n += glm::vec4(list[k].normal[0], list[k].normal[1], list[k].normal[2], list[k].normal[3]);
                }
                //n /= list.size();
                n = glm::normalize(n);
                //fprintf(stdout, "N: %f, %f, %f\n", n.x, n.y, n.z);

                for(size_t k=0; k<_vertices[i].first.size(); k++){
                    for(size_t l=0; l<_vertices[i].first[j].first.size(); l++){
                        //if(_vertices[i][j].first[l].vertID == j){
                            if(l>0){
                                glm::vec4 ln = glm::vec4(_vertices[i].first[k].first[l-1].normal[0],_vertices[i].first[k].first[l-1].normal[1],_vertices[i].first[k].first[l-1].normal[2],_vertices[i].first[k].first[l-1].normal[3]);
                                glm::vec4 cn = glm::vec4(_vertices[i].first[k].first[l].normal[0],_vertices[i].first[k].first[l].normal[1],_vertices[i].first[k].first[l].normal[2],_vertices[i].first[k].first[l].normal[3]);
                                if((glm::dot(ln, cn) != 0)){
                                    _vertices[i].first[k].first[l].normal[0] = n.x;
                                    _vertices[i].first[k].first[l].normal[1] = n.y;
                                    _vertices[i].first[k].first[l].normal[2] = n.z;
                                    _vertices[i].first[k].first[l].normal[3] = 1.0;
                                }
                            }
                            else{
                                _vertices[i].first[k].first[l].normal[0] = n.x;
                                _vertices[i].first[k].first[l].normal[1] = n.y;
                                _vertices[i].first[k].first[l].normal[2] = n.z;
                                _vertices[i].first[k].first[l].normal[3] = 1.0;
                            }
                        //}
                    }
                }
            }
        }
    }

    void CSXLoader::optimize(){
        //TODO: refactor load to do this during load (?)
        /*std::vector< std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > > > objects;

        size_t max = 0;
        for(size_t i=0; i<_vertices.size(); i++){
            for(size_t j=0; j<_vertices[i].size(); j++){
                if(_vertices[i][j].second.first > max)
                    max = _vertices[i][j].second.first;
            }
        }
        if(max == 0)
            max = 1;

        //objects.reserve(max);

        for(size_t i=0; i<max; i++){
            std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > > matGroup;
            objects.push_back(matGroup);

            for(size_t j=0; j<_vertices.size(); j++){
                for(size_t k=0; k<_vertices[j].size(); k++){
                    if(_vertices[j][k].second.first == i){
                        objects[i].push_back(_vertices[j][k]);
                    }
                }
            }

            fprintf(stdout, "Objects in Group: %i\n", objects[i].size());
        }

        fprintf(stdout, "Num Groups: %i\n", objects.size());

        VertexData newData;
        for(size_t i=0; i<objects.size(); i++){
            std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t> > > g;
            newData.push_back(g);

            for(size_t j=0; j<objects[i].size(); j++)
                newData[i].push_back(objects[i][j]);
        }
        _vertices = newData;
        objects.clear();*/
    }

}
