#include "Camera.h"

namespace Canis
{

    Camera::Camera(std::string name, glm::vec3 pos, glm::vec3 lookAt)
        : IObject(name, "camera", glm::mat4(1.0)){
        _view = glm::normalize(lookAt - pos);
        _position = pos;
        _orientation = glm::fquat();

        _pitchAccum = 0.0f;
        _yawAccum = 0.0f;

        setCamera(pos, lookAt);
    }

    void Camera::update(){
        glm::mat4 rot = glm::mat4_cast(_orientation);
        glm::mat4 trans = glm::translate(glm::mat4(), _position);
        _transform = glm::inverse(trans*rot);
        //_transform = glm::lookAt(_position, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*rot;
    }

    void Camera::setCamera(glm::vec3 pos, glm::vec3 lookAt){
        //_view = glm::normalize(lookAt - pos);
        _position = pos;
        _transform = glm::lookAt(pos, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        _orientation = glm::quat_cast(_transform);
        _view = glm::normalize(_orientation*glm::vec3(0.0f, 0.0f, 1.0f));
    }

    glm::vec3 Camera::getViewVector(){
        return _view;
    }

    glm::vec3 Camera::getPosition(){
        return _position;
    }

    glm::vec3 Camera::getAbsolutePosition(){
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

    glm::fquat Camera::getOrientation(){
        return _orientation;
    }

    void Camera::move(glm::vec3 move){
        _position += _view*move;
        update();
    }

    void Camera::strafe(float magnitude){
        _position += glm::cross(_view, glm::vec3(0.0f, 1.0f, 0.0f))*magnitude;
        update();
    }

    void Camera::rotate(float pitch, float yaw, float roll){
        glm::fquat rot;

        rot = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        _orientation = _orientation * rot;

        rot = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        _orientation = rot * _orientation;

        _view = glm::normalize(_orientation*glm::vec3(0.0f, 0.0f, 1.0f));

        update();
    }

    void Camera::setPosition(glm::vec3 pos){
        _position = pos;
        update();
    }

    void Camera::setRotation(float pitch, float yaw, float roll){
        glm::fquat rot;
        _orientation = glm::fquat();

        rot = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        _orientation = _orientation * rot;

        rot = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        _orientation = rot * _orientation;

        _view = glm::normalize(_orientation*glm::vec3(0.0f, 0.0f, 1.0f));

        update();
    }

    void Camera::setOrientation(glm::fquat orientation){
        _orientation = orientation;
        update();
    }

    void Camera::reset(){
    }

}
