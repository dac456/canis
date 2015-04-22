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

        std::vector<SceneNode*> _children;

        std::vector<IEntity*> _entities;
        //std::vector<Light*> _lights;
        std::map<std::string, Light*> _lights;
        std::vector<Camera*> _cameras;

    public:
        SceneNode(std::string name, glm::mat4 transform = glm::mat4(1.0f));
        ~SceneNode();

        void render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
        void reset(); //Override

        void attachSceneNode(SceneNode* node);
        void attachEntity(IEntity* entity);
        void attachLight(Light* light);
        void attachCamera(Camera* camera);
        
        void removeEntity(IEntity* entity);
        void removeLight(Light* light);

        std::vector<SceneNode*> getChildren();
        std::vector<IEntity*> getEntities();
        std::vector<Light*> getLights();
        std::vector<Camera*> getCameras();
        
        Light* getLight(std::string name);

        void translate(glm::vec3 translation);
        void setScale(glm::vec3 scale); //Scale signal to trigger collision rebuild?
        
    //protected:
        Scene* getParentScene();    
    
    private:
        friend class Scene;
    };

}

#endif
