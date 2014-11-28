#include "AxisAlignedBox.h"

namespace Canis
{

    AxisAlignedBox::AxisAlignedBox(){
        _min = _max = _center = glm::vec3(0.0, 0.0, 0.0);
    }

    void AxisAlignedBox::addPoint(glm::vec3 point){
        if(point.x < _min.x)
            _min.x = point.x;
        if(point.y < _min.y)
            _min.y = point.y;
        if(point.z < _min.z)
            _min.z = point.z;

        if(point.x > _max.x)
            _max.x = point.x;
        if(point.y > _max.y)
            _max.y = point.y;
        if(point.z > _max.z)
            _max.z = point.z;

        _center = glm::vec3((_max.x+_min.x)/2.0f, (_max.y+_min.y)/2.0f, (_max.z+_min.z)/2.0f);
    }

    void AxisAlignedBox::addPoint(float point[3]){
        if(point[0] < _min.x)
            _min.x = point[0];
        if(point[1] < _min.y)
            _min.y = point[1];
        if(point[2] < _min.z)
            _min.z = point[2];

        if(point[0] > _max.x)
            _max.x = point[0];
        if(point[1] > _max.y)
            _max.y = point[1];
        if(point[2] > _max.z)
            _max.z = point[2];

        _center = glm::vec3((_max.x+_min.x)/2.0f, (_max.y+_min.y)/2.0f, (_max.z+_min.z)/2.0f);
    }

    void AxisAlignedBox::addAxisAlignedBox(AxisAlignedBox box){
        if(box.getMin().x < _min.x)
            _min.x = box.getMin().x;
        if(box.getMin().y < _min.y)
            _min.y = box.getMin().y;
        if(box.getMin().z < _min.z)
            _min.z = box.getMin().z;

        if(box.getMax().x > _max.x)
            _max.x = box.getMax().x;
        if(box.getMax().y > _max.y)
            _max.y = box.getMax().y;
        if(box.getMax().z > _max.z)
            _max.z = box.getMax().z;

        _center = glm::vec3((_max.x+_min.x)/2.0f, (_max.y+_min.y)/2.0f, (_max.z+_min.z)/2.0f);
    }

    glm::vec3 AxisAlignedBox::getMin(){
        return _min;
    }

    glm::vec3 AxisAlignedBox::getMax(){
        return _max;
    }

    glm::vec3 AxisAlignedBox::getCenter(){
        return _center;
    }

    float AxisAlignedBox::getRadius(){
        float r1 = fabs(getMax().x)-fabs(getCenter().x);
        float r2 = fabs(getMax().y)-fabs(getCenter().y);
        float r3 = fabs(getMax().z)-fabs(getCenter().z);

        float i = 0.0f;
        if(r1 >= r2)
            i = r1;
        else
            i = r2;

        if(i >= r3)
            return i;
        else
            return r3;
    }

}

