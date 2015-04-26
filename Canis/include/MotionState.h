#ifndef __MOTIONSTATE_H
#define __MOTIONSTATE_H

#include "Common.h"

namespace Canis
{
    
    class CSAPI MotionState : public btMotionState{
    private:
        btTransform _trans;
        
    public:
        MotionState();
        virtual ~MotionState();
        
        void setBulletWorldTransform(glm::mat4 trans);
        glm::mat4 getBulletWorldTransform();
        
        virtual void getWorldTransform(btTransform& trans) const;
        virtual void setWorldTransform(const btTransform& trans);
    };

}

#endif
