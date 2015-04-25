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
        std::map<std::string, SceneNodePtr> _nodes;
        Camera* _activeCamera;
        
        //Maps of all objects contained in scene
        std::map<std::string, SceneNodePtr> _globalNodes;
        std::map<std::string, LightPtr> _globalLights;
        std::map<std::string, IEntityPtr> _globalEntities;

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

        void addSceneNode(SceneNodePtr node);
        void removeSceneNode(std::string name);
        
        void setActiveCamera(Camera* camera);
        Camera* getActiveCamera();

        SceneNodePtr getNode(std::string name);
        std::vector<SceneNodePtr> getNodes();
        
        bool nodeExists(std::string name);
        bool lightExists(std::string name);
        bool entityExists(std::string name);

        btDiscreteDynamicsWorld* getDynamicsWorld();
        
        std::vector<LightPtr> getLightsClosestToPoint(glm::vec4 point);
        
    private:
        void _addNode(SceneNodePtr node);
        void _removeNode(std::string name);
        void _addLight(LightPtr light);
        void _removeLight(std::string name);
        void _addEntity(IEntityPtr entity);
        void _removeEntity(std::string name);
        
        friend void SceneNode::attachSceneNode(SceneNodePtr node);
        friend void SceneNode::removeSceneNode(std::string name);
        friend void SceneNode::attachLight(LightPtr light);
        friend void SceneNode::removeLight(std::string name);
        friend void SceneNode::attachEntity(IEntityPtr entity);
        friend void SceneNode::removeEntity(std::string name);
        
    };

}

#endif
