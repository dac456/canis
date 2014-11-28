#ifndef __AXISALIGNEDBOX_H
#define __AXISALIGNEDBOX_H

#include "Common.h"

namespace Canis
{

    class CSAPI AxisAlignedBox{
    private:
        glm::vec3 _min, _max, _center;

    public:
        AxisAlignedBox();

        void addPoint(glm::vec3 point);
        void addPoint(float point[3]);

        void addAxisAlignedBox(AxisAlignedBox box);

        glm::vec3 getMin();
        glm::vec3 getMax();
        glm::vec3 getCenter();
        float getRadius();
    };

}

#endif
