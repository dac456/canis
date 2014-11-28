#include "Light.h"
#include "RenderManager.h"

namespace Canis
{

    Light::Light(std::string name, glm::vec3 diffuse, float radius, glm::mat4 transform)
        : IObject("light", transform){
        _name = name;
        _diffuse = diffuse;
        _radius = radius;
    }

    glm::vec3 Light::getPosition(){
        return glm::vec3(_transform[3][0], _transform[3][1], _transform[3][2]);
    }

    glm::vec3 Light::getAbsolutePosition(){
        if(_parent != nullptr){
            glm::mat4 absTransform = _parent->getTransform()*_transform;
            IObject* next = _parent->getParent();
            while(next != nullptr){
                absTransform = next->getTransform()*absTransform;
                next = next->getParent();
            }
            return glm::vec3(absTransform[3][0], absTransform[3][1], absTransform[3][2]);
        }
        else
            return getPosition();
    }

    std::string Light::getName(){
        return _name;
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
