#ifndef __SCENENODE_H
#define __SCENENODE_H

#include "Common.h"

#include "Mesh.h"
#include "IObject.h"

namespace Canis
{

    class CSAPI SceneNode : public IObject{
    private:
        Mesh* _marker;

        std::map<std::string, SceneNodePtr> _children;

        std::map<std::string, IEntityPtr> _entities;
        std::map<std::string, LightPtr> _lights;
        std::vector<Camera*> _cameras;

    public:
        SceneNode(std::string name, glm::mat4 transform = glm::mat4(1.0f));
        ~SceneNode();

        void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void reset(); //Override

        void attachSceneNode(SceneNodePtr node);
        void attachEntity(IEntityPtr entity);
        void attachLight(LightPtr light);
        void attachCamera(Camera* camera);
        
        void removeSceneNode(std::string name);
        void removeEntity(std::string name);
        void removeLight(std::string name);

        std::vector<SceneNodePtr> getChildren();
        std::vector<IEntityPtr> getEntities();
        std::vector<LightPtr> getLights();
        std::vector<Camera*> getCameras();
        
        SceneNodePtr getChild(std::string name);
        IEntityPtr getEntity(std::string name);
        LightPtr getLight(std::string name);

        void translate(glm::vec3 translation);
        void setScale(glm::vec3 scale); //Scale signal to trigger collision rebuild?
        
    //protected:
        ScenePtr getParentScene();    
    
    private:
        friend class Scene;
    };

}

#endif
