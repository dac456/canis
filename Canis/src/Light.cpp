#include "Light.h"
#include "RenderManager.h"

namespace Canis
{

    Light::Light(std::string name, glm::vec3 diffuse, float radius, glm::mat4 transform)
        : IObject(name, "light", transform){
        _diffuse = diffuse;
        _radius = radius;
        _scale = glm::vec3(radius, radius, radius);
    }
    
    Light::~Light(){
        std::cout << "destroying light " << getName() << std::endl;
    }

    glm::vec3 Light::getPosition(){
        return glm::vec3(_transform[3][0], _transform[3][1], _transform[3][2]);
    }

    glm::vec3 Light::getAbsolutePosition(){
        if(_parent != nullptr){
            glm::mat4 absTransform = _parent->getTransform()*_transform;
            IObjectPtr next = _parent->getParent();
            while(next != nullptr){
                absTransform = next->getTransform()*absTransform;
                next = next->getParent();
            }
            return glm::vec3(absTransform[3][0], absTransform[3][1], absTransform[3][2]);
        }
        else
            return getPosition();
    }

    void Light::setDiffuse(glm::vec3 diffuse){
        _diffuse = diffuse;
    }

    glm::vec3 Light::getDiffuse(){
        return _diffuse;
    }

    float Light::getRadius(){
        return _radius;
    }

    bool Light::operator<(Light& rhs){
        if(_brightness < rhs._brightness)
            return true;
        else
            return false;
    }

    void Light::reset(){
    }

}
