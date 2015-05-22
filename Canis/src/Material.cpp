#include "Material.h"
#include "Texture.h"
#include "GlShader.h"
#include "MaterialLoader.h"

namespace Canis
{

    Material::Material(std::string name, Technique defaultTechnique){
        _name = name;
        _currentTechnique = 0;
        _techniques.push_back(defaultTechnique);
    }

    Material::Material(MaterialLoader* matLoader){
        if(matLoader->isLoaded()){
            _name = matLoader->getData().first;
            _techniques = matLoader->getData().second;
            fprintf(stdout, "Material: %s\n", _name.c_str());
        }

        delete matLoader;
    }

    Material::~Material(){
        //should be handled by managers
        /*for(size_t i=0; i<_techniques.size(); i++)
            for(size_t j=0; j<_techniques[i].passes.size(); j++)
                for(size_t k=0; k<_techniques[i].passes[j].textures.size(); k++){
                    delete _techniques[i].passes[j].textures[k];
                    //delete _techniques[i].passes[j].shader;
                }*/
    }

    void Material::use(){
        /*GLuint shd = _techniques[0].passes[0].shader->getProgramId();
        _techniques[0].passes[0].shader->setUniformMat4f("viewMatrix", RenderManager::getSingleton().getViewMatrix());
        _techniques[0].passes[0].shader->setUniformMat4f("projectionMatrix", RenderManager::getSingleton().getProjectionMatrix());
        _techniques[0].passes[0].shader->setUniformMat4f("modelMatrix", glm::mat4(1.0f));
        _techniques[0].passes[0].shader->use();
        _techniques[0].passes[0].textures[0]->use(shd);*/
    }

    void Material::addTechnique(Technique technique){
        _techniques.push_back(technique);
    }

    bool Material::setTechnique(std::string name){
        for(size_t i=0; i<_techniques.size(); i++){
            if(_techniques[i].name.compare(name) == 0){
                _currentTechnique = i;
                return true;
            }
        }
        return false;
    }

    Technique Material::getCurrentTechnique(){
        return _techniques[_currentTechnique];
    }

    size_t Material::getId(){
        return _id;
    }

    std::string Material::getName(){
        return _name;
    }

    void Material::setName(std::string name){
        _name = name;
    }

    void Material::_setId(size_t id){
        _id = id;
    }

    std::vector<Technique> Material::getTechniques(){
        return _techniques;
    }

}
