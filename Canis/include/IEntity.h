#ifndef __IENTITY_H
#define __IENTITY_H

#include "Common.h"
#include "IObject.h"
#include "SceneNode.h"

namespace Canis
{
    
    typedef enum{
        PARAM_STRING,
        PARAM_PATH,
        PARAM_SHAPE,
        PARAM_REAL,
        PARAM_INT
    } PARAM_TYPES;

    class CSAPI IEntity : public IObject{
    private:
        std::string _type;
        std::string _name;
        bool _renderable;

    protected:
        StringMap _userParams;
        
        typedef std::map<std::string, PARAM_TYPES> ParameterMap;
        ParameterMap _paramTypes;

    public:
        IEntity(std::string type, std::string name, glm::mat4 transform = glm::mat4(1.0f), bool renderable = false, const StringMap& userParams = StringMap())
            : IObject("entity", transform){
            _type = type;
            _name = name;
            _transform = transform;
            _renderable = renderable;

            _userParams = userParams;
        }
        virtual ~IEntity(){}

        virtual void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) = 0;
        
        ParameterMap getParamTypes(){
            if(_paramTypes.empty()){
                std::cout << "WARNING: entity " << _type << " does not specify it's parameter types" << std::endl;
            }
            
            return _paramTypes;
        }

        std::string getType(){
            return _type;
        }

        std::string getName(){
            return _name;
        }

        bool isRenderable(){
            return _renderable;
        }

        virtual void reset(){} //Override
        
    protected:
        SceneNode* getParentNode(){
            if(_parent != nullptr){
                if(_parent->getType() == "node"){
                    return static_cast<SceneNode*>(_parent);
                }
                else{
                    IObject* next = _parent->getParent();
                    while(next != nullptr){
                        if(next->getType() == "node"){
                            return static_cast<SceneNode*>(next);
                        }
                    }
                }
            }
            else{
                return nullptr;
            }
        }              

    private:
        virtual void _entityAttached() = 0;

        friend class SceneNode;
    };

}

#endif
