#ifndef __CAMERA_H
#define __CAMERA_H

#include "Common.h"
#include "IObject.h"

namespace Canis
{

    class CSAPI Camera : public IObject{
    private:
        glm::vec3 _view;
        glm::vec3 _position;
        glm::fquat _orientation;

        float _pitchAccum;
        float _yawAccum;

    public:
        Camera(std::string name, glm::vec3 pos, glm::vec3 lookAt);

        void update();

        void setCamera(glm::vec3 pos, glm::vec3 lookAt);

        glm::vec3 getViewVector();
        glm::vec3 getPosition();
        glm::vec3 getAbsolutePosition();
        glm::fquat getOrientation();

        void move(glm::vec3 move);
        void strafe(float magnitude);
        void rotate(float pitch, float yaw, float roll);

        void setPosition(glm::vec3 pos);
        void setRotation(float pitch, float yaw, float roll);
        void setOrientation(glm::fquat orientation);

        void reset();

        friend class SceneNode;
    };

}

#endif
