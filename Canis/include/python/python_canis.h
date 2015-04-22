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
            Scene* sc = renderer->getActiveScene();
            if(sc == nullptr){
                std::cout << "Python get_active_scene: No active scene" << std::endl;
                return nullptr;
            }
            else{
                return sc;  
            }
        }
        else{
            return nullptr;
        }
    }
    
    void attachLight(Scene* scene, std::string node, std::string name, float r, float g, float b, float radius){
        SceneNode* n = scene->getNode(node);
        if(n != nullptr){
            n->attachLight(new Light(name, glm::vec3(r, g, b), radius));
        }
    }

    BOOST_PYTHON_MODULE(canis){
        py::class_<Scene>("scene", py::no_init)
            .def("get_name", &Scene::getName)
        ;
        
        py::def("get_active_scene", &getActiveScene, py::return_value_policy<py::reference_existing_object>());
        py::def("attach_light", &attachLight, "doc string");
    }
    
    void registerModuleSceneGraph(){
        initcanis();
    }

}

#endif
