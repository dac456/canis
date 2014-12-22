#ifndef __SCENE_H
#define __SCENE_H

#include "Common.h"
#include "IObject.h"

#include "SceneNode.h"
#include "Timer.h"

namespace Canis
{

    class CSAPI Scene : public IObject{
    private:
        std::string _name;
        std::vector<SceneNode*> _nodes;
        std::vector<Light*> _lights; //Maintain a list of lights for distance computation
        Camera* _activeCamera;

        //--Physics--//
        Timer* _timer;
        btBroadphaseInterface* _broadphase;
        btDefaultCollisionConfiguration* _collisionConfiguration;
        btCollisionDispatcher* _collisionDispatcher;
        btSequentialImpulseConstraintSolver* _solver;
        btDiscreteDynamicsWorld* _dynamicsWorld;
        
        uint64_t _lastTime;

    public:
        Scene(std::string name, glm::mat4 transform = glm::mat4(1.0f));
        ~Scene();

        void render(Camera* activeCamera, glm::mat4 projectionMatrix);
        void reset(); //Override

        void addSceneNode(SceneNode* node);
        void removeSceneNode(SceneNode* node);
        
        void setActiveCamera(Camera* camera);
        Camera* getActiveCamera();

        void setName(std::string name);
        std::string getName();
        std::vector<SceneNode*> getNodes();

        btDiscreteDynamicsWorld* getDynamicsWorld();
        
        std::vector<Light*> getLightsClosestToPoint(glm::vec4 point);
        
    private:
        void _addLight(Light* light);
        void _removeLight(Light* light);
        
        friend void SceneNode::attachLight(Light* light);
        friend void SceneNode::removeLight(Light* light);
        //friend SceneNode; //TODO: this is currently just for addLight. right idea?
        
    };

}

#endif
