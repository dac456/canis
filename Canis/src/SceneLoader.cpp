#include "SceneLoader.h"
#include "Scene.h"
#include "EntityManager.h"
#include "IEntity.h"
#include "Light.h"

namespace fs = boost::filesystem;

namespace Canis
{

    SceneLoader::SceneLoader(std::string file){
        _scene = std::make_shared<Scene>("new_untitled", glm::mat4(1.0f));
        _lastObj = _scene;
        
        _load(file);
    }
    
    SceneLoader::~SceneLoader(){
    }
    
    ScenePtr SceneLoader::getScene(){
        return _scene;
    }
    
    void SceneLoader::_load(std::string file){
        TiXmlDocument doc(file);
        if(doc.LoadFile()){
            TiXmlNode* parent = &doc;
            _loadElement(parent->FirstChild());
        }
    }
    
    void SceneLoader::_loadElement(TiXmlNode* parent){
        int t = parent->Type();
        
        switch(t){
            case TiXmlNode::TINYXML_ELEMENT:
                if(parent->ValueStr() == "scene"){
                    std::string name = parent->ToElement()->Attribute("name");
                    _scene->setName(name);
                    
                    for(TiXmlNode* child = parent->FirstChild(); child != 0; child = child->NextSibling()){
                        if(child->ValueStr() == "node"){
                            _loadNode(child, _scene);
                        }
                    }
                }
                                    
                break;
        }
    }
    
    void SceneLoader::_loadNode(TiXmlNode* parent, IObjectPtr parentObj){
        TiXmlElement* e = parent->ToElement();
        SceneNodePtr node = std::make_shared<SceneNode>(e->Attribute("name"));
        
        if(e->Attribute("script") != nullptr){
            fs::path scriptPath(e->Attribute("script"));
            Script* script = new Script(scriptPath.filename().string(), scriptPath);
            node->setScript(script);
        }
        
        if(parentObj->getType() == "scene"){
            ScenePtr sc = std::static_pointer_cast<Scene>(parentObj);
            sc->addSceneNode(node);
        }
        else if(parentObj->getType() == "node"){
            SceneNodePtr sn = std::static_pointer_cast<SceneNode>(parentObj);
            sn->attachSceneNode(node);
        }
        
        glm::mat4 transform;
        
        for(TiXmlNode* child = parent->FirstChild(); child != 0; child = child->NextSibling()){
            if(child->ValueStr() == "node"){
                _loadNode(child, node);
            }
            
            if(child->ValueStr() == "entity"){
                _loadEntity(child, node);
            }
            
            if(child->ValueStr() == "light"){
                _loadLight(child, node);
            }
            
            if(child->ValueStr() == "transform"){
                _loadMat4(child->FirstChild(), transform);
                node->setTransform(transform);
                node->setInitialTransform(transform);
            }
        }        
    }
    
    void SceneLoader::_loadEntity(TiXmlNode* parent, IObjectPtr parentObj){
        TiXmlElement* e = parent->ToElement();
        
        std::string entName = e->Attribute("name");
        std::string entType = e->Attribute("type");
        
        glm::mat4 transform;
        
        StringMap params;
        for(TiXmlNode* child = parent->FirstChild(); child != 0; child = child->NextSibling()){
            if(child->ValueStr() == "param"){
                params[child->ToElement()->Attribute("name")] = child->ToElement()->Attribute("value");
            }
            
            if(child->ValueStr() == "transform"){
                _loadMat4(child->FirstChild(), transform);
            }            
        }
        
        IEntityPtr ent = EntityManager::getSingleton().getEntityFactory(entType)->createEntity(entName, transform, params);
        
        if(e->Attribute("script") != nullptr){
            fs::path scriptPath(e->Attribute("script"));
            Script* script = new Script(scriptPath.filename().string(), scriptPath);
            ent->setScript(script);
        }        
        
        if(parentObj->getType() == "node"){
            SceneNodePtr parentNode = std::static_pointer_cast<SceneNode>(parentObj);
            parentNode->attachEntity(ent);
        }
    }
    
    void SceneLoader::_loadLight(TiXmlNode* parent, IObjectPtr parentObj){
        TiXmlElement* e = parent->ToElement();
        
        std::string lightName = e->Attribute("name");
        glm::mat4 transform;
        glm::vec3 diffuse;
        float radius;
        
        for(TiXmlNode* child = parent->FirstChild(); child != 0; child = child->NextSibling()){
            if(child->ValueStr() == "transform"){
                _loadMat4(child->FirstChild(), transform);
            }
            
            if(child->ValueStr() == "diffuse"){
                _loadVec3(child->FirstChild(), diffuse);
            }
            
            if(child->ValueStr() == "radius"){
                radius = std::stof(child->FirstChild()->ValueStr());
            }
        }
        
        LightPtr light = std::make_shared<Light>(lightName, diffuse, radius, transform);
        
        if(e->Attribute("script") != nullptr){
            fs::path scriptPath(e->Attribute("script"));
            Script* script = new Script(scriptPath.filename().string(), scriptPath);
            light->setScript(script);
        }        
        
        if(parentObj->getType() == "node"){
            SceneNodePtr parentNode = std::static_pointer_cast<SceneNode>(parentObj);
            parentNode->attachLight(light);
        }
    }
    
    void SceneLoader::_loadMat4(TiXmlNode* data, glm::mat4& mat){
        TiXmlText* text = data->ToText();
        
        std::string dataStr = data->ValueStr();
        std::vector< std::string> values = Utility::split(dataStr, ' ');
        
        for(size_t i=0; i<4; i++){
            for(size_t j=0; j<4; j++){
                mat[i][j] = std::stof(values[(4*i) + j]);
            }
        }
    }
    
    void SceneLoader::_loadVec3(TiXmlNode* data, glm::vec3& vec){
        TiXmlText* text = data->ToText();
        
        std::string dataStr = data->ValueStr();
        std::vector< std::string> values = Utility::split(dataStr, ' ');
        
        vec.x = std::stof(values[0]);
        vec.y = std::stof(values[1]);
        vec.z = std::stof(values[2]);        
    }

}
