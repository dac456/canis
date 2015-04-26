#ifndef __PYTHON_CANIS_H
#define __PYTHON_CANIS_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "Common.h"

#include "Scene.h"
#include "Engine.h"
#include "Renderer.h"
#include "Light.h"

namespace py = boost::python;

namespace Canis
{
    
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

    BOOST_PYTHON_MODULE(canis){
        py::class_<Scene>("scene", py::no_init)
            .def("get_name", &Scene::getName)
        ;
        
        py::def("get_active_scene", &getActiveScene, py::return_value_policy<py::reference_existing_object>());
        py::def("attach_light", &attachLight, "doc string");
        py::def("remove_light", &removeLight, "doc string");
    }
    
    void registerModuleCanisMain(){
        initcanis();
    }

}

#endif
