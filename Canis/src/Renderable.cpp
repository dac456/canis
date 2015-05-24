#include "Renderable.h"

namespace Canis
{
    
    Renderable::Renderable(std::string name, std::vector<VertexObject*> objects){
        _name = name;
        _vertexObjects = objects;
    }
    
    Renderable::~Renderable(){
    }
    
    /*void Renderable::update(RenderablePtr renderable){
        _transform = renderable->getTransform();
        _normalMatrix = renderable->getNormalMatrix();
        
        _lightPositions = renderable->getLightPositions();
        _lightColors = renderable->getLightColors();
        _lightRadii = renderable->getLightRadii();
    }*/
    
    std::string Renderable::getName(){
        return _name;
    }
    
    std::vector<VertexObject*> Renderable::getVertexObjects(){
        return _vertexObjects;
    }
    
    glm::mat4 Renderable::getTransform(){
        return _transform;
    }
    
    void Renderable::setTransform(glm::mat4 transform){
        _transform = transform;
    }
    
    glm::mat3 Renderable::getNormalMatrix(){
        return _normalMatrix;
    }
    
    void Renderable::setNormalMatrix(glm::mat3 normalMatrix){
        _normalMatrix = normalMatrix;
    }
    
    glm::mat4 Renderable::getLightPositions(){
        return _lightPositions;
    }
    
    void Renderable::setLightPositions(glm::mat4 lightPositions){
        _lightPositions = lightPositions;
    }
    
    glm::mat4 Renderable::getLightColors(){
        return _lightColors;
    }
    
    void Renderable::setLightColors(glm::mat4 lightColors){
        _lightColors = lightColors;
    }
    
    glm::vec4 Renderable::getLightRadii(){
        return _lightRadii;
    }
    
    void Renderable::setLightRadii(glm::vec4 lightRadii){
        _lightRadii = lightRadii;
    }

}