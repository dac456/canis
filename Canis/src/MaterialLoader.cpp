#include <boost/filesystem.hpp>
#include "MaterialLoader.h"
#include "Material.h"
#include "Texture.h"
#include "GlShaderManager.h"
#include "TextureManager.h"

using namespace boost;

namespace Canis
{

    MaterialLoader::MaterialLoader(std::string fileName) : IXmlLoader(){
        load("./Media/Materials/"+fileName);
    }

    MaterialLoader::~MaterialLoader(){
        for(size_t i=0; i<_techniques.size(); i++){
            for(size_t j=0; j<_techniques[i].passes.size(); j++){
                _techniques[i].passes[j].textures.clear();
            }
            _techniques[i].passes.clear();
        }
        _techniques.clear();
    }

    std::pair<std::string, std::vector<Technique>> MaterialLoader::getData(){
        std::pair<std::string, std::vector<Technique>> out;
        if(isLoaded()){
            out.first = _name;
            out.second = _techniques;
        }

        return out;
    }

    void MaterialLoader::stepAttributes(TiXmlElement* element){
        TiXmlAttribute* attrib = element->FirstAttribute();

        if(strcmp(element->Value(), "material") == 0){
            while(attrib){
                if(strcmp(attrib->Name(), "name") == 0)
                    _name = attrib->Value();
                attrib = attrib->Next();
            }
        }
        if(strcmp(element->Value(), "technique") == 0){
            Technique t;
            while(attrib){
                if(strcmp(attrib->Name(), "name") ==0)
                    t.name = attrib->Value();
                attrib = attrib->Next();
            }
            _techniques.push_back(t);
        }
        if(strcmp(element->Value(), "pass") == 0){
            Pass p;
            while(attrib){
                if(strcmp(attrib->Name(), "name") == 0)
                    p.name = attrib->Value();
                attrib = attrib->Next();
            }

            //Set defaults
            p.blend = false;
            p.diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            size_t current = _techniques.size() - 1;
            _techniques[current].passes.push_back(p);
        }
        if(strcmp(element->Value(), "shader") == 0){
            std::string name;
            while(attrib){
                if(strcmp(attrib->Name(), "name") == 0)
                    name = attrib->Value();
                attrib = attrib->Next();
            }
            size_t currentTechnique = _techniques.size() - 1;
            size_t currentPass = _techniques[currentTechnique].passes.size() - 1;
            _techniques[currentTechnique].passes[currentPass].shader = GlShaderManager::getSingleton().getShader(name);
        }
        if(strcmp(element->Value(), "texture") == 0){
            GLuint unit;
            std::string file = "";
            std::string name = "";
            while(attrib){
                if(strcmp(attrib->Name(), "file") == 0)
                    file = attrib->Value();
                if(strcmp(attrib->Name(), "name") == 0)
                    name = attrib->Value();
                if(strcmp(attrib->Name(), "unit") == 0)
                    unit = atoi(attrib->Value());
                attrib = attrib->Next();
            }

            file = Utility::findAndReplace(file, "\\", "/");

            std::string textureName = "";
            if(file.compare("") != 0)
                textureName = filesystem::path(file).filename().string();
            else if(name.compare("") != 0)
                textureName = name;

            Texture* tex = TextureManager::getSingleton().getTexture(textureName);
            if(tex == nullptr){
                tex = new Texture(textureName, file, unit);
                TextureManager::getSingleton().addTexture(tex);
            }

            size_t currentTechnique = _techniques.size() - 1;
            size_t currentPass = _techniques[currentTechnique].passes.size() - 1;
            _techniques[currentTechnique].passes[currentPass].textures.push_back(tex->getId());
        }
        if(strcmp(element->Value(), "blend") == 0){
            size_t currentTechnique = _techniques.size() - 1;
            size_t currentPass = _techniques[currentTechnique].passes.size() - 1;

            _techniques[currentTechnique].passes[currentPass].blend = true;
            while(attrib){
                if(strcmp(attrib->Name(), "src") == 0){
                    if(strcmp(attrib->Value(), "one") == 0)
                        _techniques[currentTechnique].passes[currentPass].blendSrc = GL_ONE;
                }
                if(strcmp(attrib->Name(), "dst") == 0){
                    if(strcmp(attrib->Value(), "one") == 0)
                        _techniques[currentTechnique].passes[currentPass].blendDst = GL_ONE;
                }
                attrib = attrib->Next();
            }
        }
        if(strcmp(element->Value(), "diffuse") == 0){
            size_t currentTechnique = _techniques.size() - 1;
            size_t currentPass = _techniques[currentTechnique].passes.size() - 1;

            while(attrib){
                if(strcmp(attrib->Name(), "r") == 0)
                    _techniques[currentTechnique].passes[currentPass].diffuse.r = atof(attrib->Value());
                if(strcmp(attrib->Name(), "g") == 0)
                    _techniques[currentTechnique].passes[currentPass].diffuse.g = atof(attrib->Value());
                if(strcmp(attrib->Name(), "b") == 0)
                    _techniques[currentTechnique].passes[currentPass].diffuse.b = atof(attrib->Value());
                if(strcmp(attrib->Name(), "a") == 0)
                    _techniques[currentTechnique].passes[currentPass].diffuse.a = atof(attrib->Value());

                attrib = attrib->Next();
            }
        }

    }

}
