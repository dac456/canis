#include "MotionState.h"

namespace Canis
{
    
    MotionState::MotionState(){
    }
    
    MotionState::~MotionState(){
    }
    
    void MotionState::setBulletWorldTransform(glm::mat4 trans){
        _trans.setFromOpenGLMatrix(glm::value_ptr(trans));
    }
    
    glm::mat4 MotionState::getBulletWorldTransform(){
        btScalar m[16];
        _trans.getOpenGLMatrix(m);   
        
        return glm::make_mat4(m);     
    }
    
    void MotionState::getWorldTransform(btTransform& trans) const{
        trans = _trans;
    }
    
    void MotionState::setWorldTransform(const btTransform& trans){
        _trans = trans;
    }

}
