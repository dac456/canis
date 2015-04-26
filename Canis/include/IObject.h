#ifndef __IOBJECT_H
#define __IOBJECT_H

#include "Common.h"
#include "Engine.h"
#include "Script.h"
#include "ScriptManager.h"

#include "MotionState.h"

namespace Canis
{

    //TODO: should there be a seperate motion state class?
    //TODO: should each IObject define it's type/class by a string?
    class CSAPI IObject : public std::enable_shared_from_this<IObject>{
    private:
        std::string _name;
        std::string _type;
        Script* _script; //TODO: multiple?

    protected:
        IObjectPtr _parent;
        glm::mat4 _transform, _initialTransform;
        glm::vec3 _scale;

        MotionState _motionState;
        
    public:
        IObject(std::string name, std::string type, glm::mat4 transform = glm::mat4(1.0f)){
            _name = name;
            _type = type;
            _script = nullptr;

            _parent = nullptr;
            _transform = transform;
            _initialTransform = _transform;
            _scale = glm::vec3(1.0f, 1.0f, 1.0f);

            _resetConnection = Engine::getSingleton()._connectReset(boost::bind(&IObject::reset, this));
        }
        virtual ~IObject(){
            _resetConnection.disconnect();
            unsetScript(); //TODO: fix segfault on exit
        }
        
        std::shared_ptr<IObject> getptr(){
            return shared_from_this();
        }        
                
        void setScript(Script* script){
            _script = script;
            _script->setOwner(this);
            
            ScriptManager::getSingleton().run(script, "on_set_script");
        }
        
        void unsetScript(){
            if(_script != nullptr){
                ScriptManager::getSingleton().run(_script, "on_unset_script");
                
                _script->setOwner(nullptr);
                delete _script;
            }
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

        IObjectPtr getParent(){
            return _parent;
        }

        virtual void setTransform(glm::mat4 transform){
            _transform = transform;
        }

        glm::mat4 getTransform(){
            return _transform;
        }

        glm::mat4 getAbsoluteTransform(){
            if(_parent != nullptr){
                glm::mat4 absTransform = _parent->getTransform()*_transform;
                IObjectPtr next = _parent->getParent();
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
        
    private:
        boost::signals2::connection _resetConnection;
    };

}

#endif
