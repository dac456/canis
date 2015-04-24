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
        bool _renderable;

    protected:
        StringMap _params;
        
        typedef std::map<std::string, PARAM_TYPES> ParameterMap;
        ParameterMap _paramTypes;
        std::vector<std::string> _paramUpdateList;

    public:
        IEntity(std::string type, std::string name, glm::mat4 transform = glm::mat4(1.0f), bool renderable = false)
            : IObject(name, "entity", transform){
            _type = type;
            _transform = transform;
            _renderable = renderable;
        }
        virtual ~IEntity(){}

        virtual void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) = 0;
        
        ParameterMap getParamTypes(){
            if(_paramTypes.empty()){
                std::cout << "WARNING: entity " << _type << " does not specify it's parameter types" << std::endl;
            }
            
            return _paramTypes;
        }
        
        void setParam(std::string name, std::string value){
            _params[name] = value;
            _paramUpdateList.push_back(name);
        }
        
        void setParams(StringMap params){
            for(auto pair : params){
                _params[pair.first] = pair.second;
                _paramUpdateList.push_back(pair.first);
            }
        }
        
        std::string getParam(std::string name){
            return _params[name];
        }
        
        StringMap getParams(){
            return _params;
        }

        std::string getType(){
            return _type;
        }

        bool isRenderable(){
            return _renderable;
        }

        virtual void reset(){} //Override
        
    protected:
        SceneNodePtr getParentNode(){
            if(_parent != nullptr){
                if(_parent->getType() == "node"){
                    return std::static_pointer_cast<SceneNode>(_parent);
                }
                else{
                    IObjectPtr next = _parent->getParent();
                    while(next != nullptr){
                        if(next->getType() == "node"){
                            return std::static_pointer_cast<SceneNode>(next);
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
