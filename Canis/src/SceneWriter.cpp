#include "SceneWriter.h"
#include "Scene.h"
#include "SceneNode.h"
#include "IEntity.h"
#include "Light.h"

namespace Canis
{
    
    SceneWriter::SceneWriter(Scene* scene){
        if(scene){
            TiXmlElement* root = new TiXmlElement("scene");
            root->SetAttribute("name", scene->getName());
            root->SetAttribute("version", "0.1");
            
            for(auto n : scene->getNodes()){
                _writeNode(root, n);
            }
            
            _doc.LinkEndChild(root);
        }
    }
    
    SceneWriter::~SceneWriter(){
    }
    
    void SceneWriter::save(std::string file){
        _doc.SaveFile(file);
    }
    
    void SceneWriter::_writeNode(TiXmlElement* parent, SceneNode* node){
        TiXmlElement* e = new TiXmlElement("node");
        e->SetAttribute("name", node->getName());
        
        TiXmlElement* transform = new TiXmlElement("transform");
        glm::mat4 t = node->getTransform();
        
        std::stringstream ss;
        for(size_t i=0; i<4; i++){
            for(size_t j=0; j<4; j++){
                ss << t[i][j] << " ";
            }
        }
        TiXmlText* transStr = new TiXmlText(ss.str());
        transform->LinkEndChild(transStr);
        e->LinkEndChild(transform);        
        
        for(auto ent : node->getEntities()){
            _writeEntity(e, ent);
        }
        
        for(auto light : node->getLights()){
            _writeLight(e, light);
        }
        
        for(auto n : node->getChildren()){
            _writeNode(e, n);
        }
        
        parent->LinkEndChild(e);        
    }
    
    void SceneWriter::_writeEntity(TiXmlElement* parent, IEntity* ent){
        TiXmlElement* entElem = new TiXmlElement("entity");
        entElem->SetAttribute("name", ent->getName());
        entElem->SetAttribute("type", ent->getType());
        
        TiXmlElement* transform = new TiXmlElement("transform");
        glm::mat4 t = ent->getTransform();
        
        std::stringstream ss;
        for(size_t i=0; i<4; i++){
            for(size_t j=0; j<4; j++){
                ss << t[i][j] << " ";
            }
        }
        TiXmlText* transStr = new TiXmlText(ss.str());
        transform->LinkEndChild(transStr);
        entElem->LinkEndChild(transform);
        
        for(auto pair : ent->getParams()){
            TiXmlElement* param = new TiXmlElement("param");
            param->SetAttribute("name", pair.first);
            param->SetAttribute("value", pair.second);
            entElem->LinkEndChild(param);
        }
        
        parent->LinkEndChild(entElem);        
    }        
    
    void SceneWriter::_writeLight(TiXmlElement* parent, Light* light){
        TiXmlElement* lightElem = new TiXmlElement("light");
        lightElem->SetAttribute("name", light->getName());
        
        TiXmlElement* transform = new TiXmlElement("transform");
        glm::mat4 t = light->getTransform();
        
        std::stringstream ss;
        for(size_t i=0; i<4; i++){
            for(size_t j=0; j<4; j++){
                ss << t[i][j] << " ";
            }
        }
        TiXmlText* transStr = new TiXmlText(ss.str());
        transform->LinkEndChild(transStr);
        lightElem->LinkEndChild(transform);
        
        TiXmlElement* color = new TiXmlElement("diffuse");
        
        std::stringstream colStr;
        colStr << light->getDiffuse().x << " " << light->getDiffuse().y << " " << light->getDiffuse().z;
        TiXmlText* colText = new TiXmlText(colStr.str());
        color->LinkEndChild(colText);
        
        TiXmlElement* radius = new TiXmlElement("radius");
        
        std::stringstream radStr;
        radStr << light->getRadius();
        TiXmlText* radText = new TiXmlText(radStr.str());
        radius->LinkEndChild(radText);
        
        lightElem->LinkEndChild(color);
        lightElem->LinkEndChild(radius);
        
        parent->LinkEndChild(lightElem);
    }
    
}
