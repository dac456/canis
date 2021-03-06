#ifndef __LIGHT_H
#define __LIGHT_H

#include "Common.h"
#include "IObject.h"

namespace Canis
{

    typedef enum{
        POINT,
        DIRECTIONAL
    } LIGHT_TYPE;

    class CSAPI Light : public IObject{
    private:
        glm::vec3 _diffuse;
        float _radius;

    protected:
        float _brightness;

    public:
        Light(std::string name, glm::vec3 diffuse, float radius, glm::mat4 transform = glm::mat4(1.0f));
        virtual ~Light();

        glm::vec3 getPosition();
        glm::vec3 getAbsolutePosition();
        
        void setDiffuse(glm::vec3 diffuse);
        glm::vec3 getDiffuse();
        
        float getRadius();

        void reset();

        bool operator<(Light& rhs);

        friend class SceneNode;
        friend class Scene;
    };

}

#endif
