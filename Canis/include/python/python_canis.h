#ifndef __PYTHON_CANIS_H
#define __PYTHON_CANIS_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "Common.h"

#include "Scene.h"
#include "Engine.h"
#include "Renderer.h"
#include "Light.h"
#include "MeshManager.h"
#include "AssimpLoader.h"

namespace py = boost::python;

namespace Canis
{
    
    bool dynamicsEnabled(){
        return Engine::getSingleton().isDynamicsEnabled();
    }
    
    Scene* getActiveScene(){
        Renderer* renderer = Engine::getSingleton().getRenderer();
        if(renderer != nullptr){
            ScenePtr sc = renderer->getActiveScene();
            if(sc == nullptr){
                std::cout << "Python get_active_scene: No active scene" << std::endl;
                return nullptr;
            }
            else{
                return sc.get();  
            }
        }
        else{
            return nullptr;
        }
    }
    
    void loadMesh(std::string name, std::string file){
        MeshPtr mesh = std::make_shared<Mesh>(name, new AssimpLoader(file));
        
        if(mesh){
            MeshManager::getSingleton().addMesh(mesh);
        }
    }
    
    void drawMesh(Scene* scene, std::string name, float px, float py, float pz){
        MeshPtr mesh = MeshManager::getSingleton().getMesh(name);
        
        if(mesh){
            scene->drawMesh(mesh, glm::translate(glm::vec3(px, py, pz)));
        }
    }
    
    void attachLight(Scene* scene, std::string node, std::string name, float r, float g, float b, float radius){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n != nullptr){
            n->attachLight(std::make_shared<Light>(name, glm::vec3(r, g, b), radius));
        }
    }
    
    void removeLight(Scene* scene, std::string node, std::string name){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n != nullptr){
            n->removeLight(name);
        }
    }
    
    void setNodeTransform(Scene* scene, std::string node, float px, float py, float pz, float yaw, float pitch, float roll){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n){
            glm::mat4 trans = glm::eulerAngleYXZ(yaw, pitch, roll)*glm::translate(glm::vec3(px, py, pz));
            n->setTransform(trans);
        }
    }
    
    py::tuple getNodePosition(Scene* scene, std::string node){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n){        
            glm::mat4 trans = n->getAbsoluteTransform();
            return py::make_tuple(trans[3][0], trans[3][1], trans[3][2]);
        }
    }

    BOOST_PYTHON_MODULE(canis){
        py::class_<Scene>("scene", py::no_init)
            .def("get_name", &Scene::getName)
        ;
        
        py::def("dynamics_enabled", &dynamicsEnabled);
        py::def("get_active_scene", &getActiveScene, py::return_value_policy<py::reference_existing_object>());
        
        py::def("load_mesh", &loadMesh);
        py::def("draw_mesh", &drawMesh);
        
        py::def("attach_light", &attachLight, "doc string");
        py::def("remove_light", &removeLight, "doc string");
        
        py::def("set_node_transform", &setNodeTransform);
        py::def("get_node_position", &getNodePosition);
    }
    
    void registerModuleCanisMain(){
        initcanis();
    }

}

#endif
