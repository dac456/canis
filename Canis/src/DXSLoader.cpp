#include "DXSLoader.h"
#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "Material.h"
#include "Texture.h"
#include "GlShaderManager.h"
#include "GlShader.h"

namespace Canis
{

    DXSLoader::DXSLoader(){
        _isLoaded = false;
        _loadingVertList = false;
        _hasNormals = false;
        _highestVertex = 0;
        //_currentPrimitive = -1;
        //_currentPoly = -1;
        _currentMaterial = -1;
    }
    DXSLoader::DXSLoader(std::string fileName){
        _isLoaded = false;
        _loadingVertList = false;
        _hasNormals = false;
        _highestVertex = 0;
        //_currentPrimitive = -1;
        //_currentPoly = -1;
        _currentMaterial = -1;

        load(fileName);
        calculateNormals(true);
    }

    void DXSLoader::load(std::string fileName){
        if(!_isLoaded){
            TiXmlDocument doc(fileName.c_str());
            if(doc.LoadFile()){
                step(&doc);
            }
            _isLoaded = true;
        }
    }

    void DXSLoader::step(TiXmlNode* parent){
        TiXmlNode* child;

        switch(parent->Type()){
        case TiXmlNode::TINYXML_DOCUMENT:
            break;

        case TiXmlNode::TINYXML_ELEMENT:
            stepAttributes(parent->ToElement());
            break;

        default:
            break;

        }

        for(child=parent->FirstChild(); child != 0; child = child->NextSibling())
            step(child);
    }

    void DXSLoader::stepAttributes(TiXmlElement* element){
        TiXmlAttribute* attrib = element->FirstAttribute();

        if(strcmp(element->Value(), "material") == 0){
            DXSMaterial m;
            while(attrib){
                if(strcmp(attrib->Name(), "id") == 0){
                    m.id = atoi(attrib->Value());
                    _currentMaterial++;
                }
                if(strcmp(attrib->Name(), "name") == 0){
                    m.name = std::string(attrib->Value());
                    boost::algorithm::to_lower(m.name);
                    fprintf(stdout, "Loaded Mat: %s\n", m.name.c_str());
                }
                attrib = attrib->Next();
            }
            _dxsMaterials.push_back(m);
        }
        if(strcmp(element->Value(), "texture") == 0){
            //while(attrib){
            //  if(strcmp(attrib->Name(), "file") == 0)
                    _dxsMaterials[_currentMaterial].texture = attrib->Value();
            //  attrib = attrib->Next();
            //}
        }

        if(strcmp(element->Value(), "primitive") == 0){
            DXSPrimitive p;
            //while(attrib){
            //  if(strcmp(attrib->Name(), "id") == 0){
                    p.id = atoi(attrib->Value());
                    //_currentPrimitive++;
                    //_currentPoly = -1;
                    //std::vector< std::pair<std::vector<Vertex>, std::pair<size_t, size_t>> > v;
                    //_vertices.push_back(v);
            //  }
            //  attrib = attrib->Next();
            //}
            _dxsPrimitives.push_back(p);
            
        }
        if(strcmp(element->Value(), "vertices") == 0){
            _loadingVertList = true;
        }
        if(strcmp(element->Value(), "polygons") == 0){
            _loadingVertList = false;
        }
        if(strcmp(element->Value(), "poly") == 0){
            //_currentPoly++;
            //std::pair<std::vector<Vertex>, std::pair<size_t, size_t>> o;
            //_vertices[_currentPrimitive].push_back(o);
            //_vertices[_currentPrimitive].emplace_back(o);

            //while(attrib){
            //  if(strcmp(attrib->Name(), "mid") == 0)
                    _currentMaterial = atoi(attrib->Value());
            //  attrib = attrib->Next();
            //}

            //if(!MaterialManager::instance().exists(name)
            ////newID = addMaterial(material)
            //else
            ///newID = getMaterial(name).id;
            Material* mat = MaterialManager::getSingleton().getMaterial(_dxsMaterials[_currentMaterial-1].name);
            size_t id = 0;
            if(mat == nullptr){
                mat = new Material(new MaterialLoader(_dxsMaterials[_currentMaterial-1].name+".material"));
                if(!mat->getTechniques().empty())
                    id = MaterialManager::getSingleton().addMaterial(mat);
                        //id = MaterialManager::getSingleton().addMaterial(mat);
            }
            else
                id = mat->getId();

            _currentGroup = id;
            if((id+1) > _vertices.size()){
                GroupData matGroup;
                _vertices.resize(id+1, matGroup);
            }

            ObjectData faceObj;
            _vertices[_currentGroup].first.push_back(faceObj);
            _currentFace = _vertices[_currentGroup].first.size()-1;

            /*if(mat == nullptr){
                Technique t;
                Pass p;
                Texture* tex = new Texture(_dxsMaterials[_currentMaterial].texture, 0);
                p.textures.push_back(tex);

                GlShader* shd = GlShaderManager::getSingleton().getShader("basic");
                if(shd == nullptr){
                    shd = new GlShader("basic", "basicVert", "basicFrag");
                    GlShaderManager::getSingleton().addShader(shd);
                    p.shader = shd;
                }
                else
                    p.shader = shd;

                t.passes.push_back(p);

                mat = new Material(_dxsMaterials[_currentMaterial].name, t);
                id = MaterialManager::getSingleton().addMaterial(mat);
            }
            else{
                id = mat->getId();
            }*/

            //Check if material exists in manager (by name), if not add material, and set 'second' to material id from manager
            //Uses _currentMaterial to look up dxsMaterial and create Canis Material
            //fprintf(stdout, "MATERIAL: %i, %s\n", id, _dxsMaterials[_currentMaterial].name.c_str());
            _vertices[_currentGroup].second = id; //substitute for corresponding manager ID (newID)
        }
        if(strcmp(element->Value(), "vertex") == 0){
            if(_loadingVertList){
                DXSVertex v;
                while(attrib){
                    if(strcmp(attrib->Name(), "id") == 0)
                        v.id = atoi(attrib->Value());
                    if(strcmp(attrib->Name(), "x") == 0)
                        v.p.x = atof(attrib->Value())/64.0f;
                    if(strcmp(attrib->Name(), "y") == 0)
                        v.p.y = atof(attrib->Value())/64.0f;
                    if(strcmp(attrib->Name(), "z") == 0)
                        v.p.z = atof(attrib->Value())/64.0f;
                    attrib = attrib->Next();
                }
                if(v.id > _highestVertex)
                    _highestVertex = v.id;
                _dxsPrimitives[_dxsPrimitives.size()-1].v.push_back(v);
            }
            else{
                size_t idx = -1;
                glm::vec2 uv;
                while(attrib){
                    if(strcmp(attrib->Name(), "vid") == 0)
                        idx = atoi(attrib->Value());
                    if(strcmp(attrib->Name(), "u0") == 0)
                        uv.s = atof(attrib->Value());
                    if(strcmp(attrib->Name(), "v0") == 0)
                        uv.t = atof(attrib->Value());
                    attrib = attrib->Next();
                }

                Vertex v;
                v.vertex[0] = _dxsPrimitives[_dxsPrimitives.size()-1].v[idx].p.x;
                v.vertex[1] = _dxsPrimitives[_dxsPrimitives.size()-1].v[idx].p.y;
                v.vertex[2] = _dxsPrimitives[_dxsPrimitives.size()-1].v[idx].p.z;
                v.vertex[3] = 1.0f;
                v.texCoord[0] = uv.s;
                v.texCoord[1] = uv.t;
                v.vertID = idx;
                _vertices[_currentGroup].first[_currentFace].first.push_back(v);
                //_vertices[_currentPrimitive][_currentPoly].second = _currentMaterial;
            }
        }
    }

    void DXSLoader::calculateNormals(bool averageNormals){
        for(size_t i=0; i<_vertices.size(); i++){
            for(size_t j=0; j<_vertices[i].first.size(); j++){
                Vertex v1 = _vertices[i].first[j].first[0];
                Vertex v2 = _vertices[i].first[j].first[1];
                Vertex v3 = _vertices[i].first[j].first[2];

                glm::vec3 a = glm::vec3(v2.vertex[0], v2.vertex[1], v2.vertex[2]) - glm::vec3(v1.vertex[0], v1.vertex[1], v1.vertex[2]);
                glm::vec3 b = glm::vec3(v3.vertex[0], v3.vertex[1], v3.vertex[2]) - glm::vec3(v1.vertex[0], v1.vertex[1], v1.vertex[2]);

                glm::vec3 n = glm::cross(a, b);
                //n = glm::normalize(n);

                for(size_t k=0; k<_vertices[i].first[j].first.size(); k++){
                    _vertices[i].first[j].first[k].normal[0] = -n.x;
                    _vertices[i].first[j].first[k].normal[1] = -n.y;
                    _vertices[i].first[j].first[k].normal[2] = -n.z;
                    _vertices[i].first[j].first[k].normal[3] = 1.0;
                }
            }

            //Average normals
            if(averageNormals){
                for(size_t j=0; j<_highestVertex; j++){
                    std::vector<Vertex> list;

                    for(size_t k=0; k<_vertices[i].first.size(); k++){
                        for(size_t l=0; l<_vertices[i].first[k].first.size(); l++){
                            if(_vertices[i].first[k].first[l].vertID == j){
                                    list.push_back(_vertices[i].first[k].first[l]);
                            }
                        }
                    }

                    glm::vec4 n;
                    for(size_t k=0; k<list.size(); k++){
                        n += glm::vec4(list[k].normal[0], list[k].normal[1], list[k].normal[2], list[k].normal[3]);
                    }
                    n /= list.size();
                    n = glm::normalize(n);
                    //fprintf(stdout, "N: %f, %f, %f\n", n.x, n.y, n.z);

                    for(size_t k=0; k<_vertices[i].first.size(); k++){
                        for(size_t l=0; l<_vertices[i].first[k].first.size(); l++){
                            if(_vertices[i].first[k].first[l].vertID == j){
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
                            }
                        }
                    }
                }
            }
        }

        _hasNormals = true;
    }

    VertexData DXSLoader::getData(){
        //if(_isLoaded)
            return _vertices;
    }
}
