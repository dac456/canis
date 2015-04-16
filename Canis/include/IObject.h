#ifndef __IOBJECT_H
#define __IOBJECT_H

#include "Common.h"
#include "Engine.h"
#include "Script.h"

namespace Canis
{

    //TODO: should there be a seperate motion state class?
    //TODO: should each IObject define it's type/class by a string?
    class CSAPI IObject : public btMotionState {
    private:
        std::string _name;
        std::string _type;
        Script* _script; //TODO: multiple?

    protected:
        IObject* _parent;
        glm::mat4 _transform, _initialTransform;
        glm::vec3 _scale;

        btTransform _dynamicsTransform;

    public:
        IObject(std::string name, std::string type, glm::mat4 transform = glm::mat4(1.0f)){
            _name = name;
            _type = type;
            _script = nullptr;

            _parent = nullptr;
            _transform = transform;
            _initialTransform = _transform;
            _scale = glm::vec3(1.0f, 1.0f, 1.0f);

            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_transform));

            _resetConnection = Engine::getSingleton()._connectReset(boost::bind(&IObject::reset, this));
        }
        virtual ~IObject(){
            _resetConnection.disconnect();
            if(_script != nullptr){
                delete _script;
            }
        }
        
        void setScript(Script* script){
            _script = script;
            _script->setOwner(this);
        }
        
        Script* getScript(){
            return _script;
        }
        
        std::string getName(){
            return _name;
        }
        
        void setName(std::string name){
            _name = name;
        }

        std::string getType(){
            return _type;
        }

        IObject* getParent(){
            return _parent;
        }

        virtual void setTransform(glm::mat4 transform){
            _transform = transform;
            _dynamicsTransform.setFromOpenGLMatrix(glm::value_ptr(_transform));
        }

        glm::mat4 getTransform(){
            return _transform;
        }

        glm::mat4 getAbsoluteTransform(){
            if(_parent != nullptr){
                glm::mat4 absTransform = _parent->getTransform()*_transform;
                IObject* next = _parent->getParent();
                while(next != nullptr){
                    absTransform = next->getTransform()*absTransform;
                    next = next->getParent();
                }

                return absTransform;
            }
            else{
                return _transform;
            }
        }

        virtual void setInitialTransform(glm::mat4 transform){
            _initialTransform = transform;
        }

        glm::mat4 getInitialTransform(){
            return _initialTransform;
        }
        
        virtual void setScale(glm::vec3 scale){
            //_transform = glm::scale(scale)*_transform;
            _scale = scale;
        }        
        
        glm::vec3 getScale(){
            return _scale;
        }

        virtual void reset() = 0; //Should be pure virtual?

        //--Bullet overrides--//
        virtual void getWorldTransform(btTransform& trans) const{
            trans = _dynamicsTransform;
        }

        virtual void setWorldTransform(const btTransform& trans){
            _dynamicsTransform = trans;

            //TODO: this seems like it is probably slow and wasteful
            btScalar m[16];
            trans.getOpenGLMatrix(m);
            _transform = glm::make_mat4(m);
        }

    private:
        boost::signals2::connection _resetConnection;
    };

}

#endif
