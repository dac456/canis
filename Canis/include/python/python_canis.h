#ifndef __PYTHON_CANIS_H
#define __PYTHON_CANIS_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "Common.h"

#include "Scene.h"
#include "Engine.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Light.h"
#include "MeshManager.h"
#include "Material.h"
#include "AssimpLoader.h"

namespace py = boost::python;

namespace Canis
{
    
    bool dynamicsEnabled(){
        return Engine::getSingleton().isDynamicsEnabled();
    }
    
    ScenePtr getActiveScene(){
        Renderer* renderer = Engine::getSingleton().getRenderer();
        if(renderer != nullptr){
            ScenePtr sc = renderer->getActiveScene();
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
    
    void loadMesh(std::string name, std::string file){
        MeshPtr mesh = std::make_shared<Mesh>(name, new AssimpLoader(file));
        
        if(mesh){
            MeshManager::getSingleton().addMesh(mesh);
        }
    }
    
    void drawMesh(ScenePtr scene, std::string name, float px, float py, float pz){
        MeshPtr mesh = MeshManager::getSingleton().getMesh(name);
        
        if(mesh){
            scene->drawMesh(mesh, glm::translate(glm::vec3(px, py, pz)));
        }
    }
    
    std::vector<RenderablePtr> enqueueMesh(std::string name, float px, float py, float pz){
        MeshPtr mesh = MeshManager::getSingleton().getMesh(name);
        
        if(mesh){
            std::vector<RenderablePtr> renderables = mesh->toRenderable();
            for(auto r : renderables){
                r->setTransform(glm::translate(glm::vec3(px, py, pz)));
                Engine::getSingleton().getRenderer()->enqueueRenderable(r, 0);
            }
            //mesh->setTransform(glm::translate(glm::vec3(px, py, pz)));
            //mesh->enqueue();
            
            return renderables;
        }
    }    
    
    void dequeueMesh(std::vector<RenderablePtr> renderables){
        //TODO
    }
    
    void updateMeshTransform(ScenePtr scene, std::vector<RenderablePtr> renderables, float px, float py, float pz){
        for(auto r : renderables){
            r->setTransform(glm::translate(glm::vec3(px, py, pz)));
            Engine::getSingleton().getRenderer()->updateRenderable(r);
        }
    }
    
    void attachLight(ScenePtr scene, std::string node, std::string name, float r, float g, float b, float radius){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n != nullptr){
            n->attachLight(std::make_shared<Light>(name, glm::vec3(r, g, b), radius));
        }
    }
    
    void removeLight(ScenePtr scene, std::string node, std::string name){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n != nullptr){
            n->removeLight(name);
        }
    }
    
    void setNodeTransform(ScenePtr scene, std::string node, float px, float py, float pz, float yaw, float pitch, float roll){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n){
            glm::mat4 trans = glm::eulerAngleYXZ(yaw, pitch, roll)*glm::translate(glm::vec3(px, py, pz));
            n->setTransform(trans);
        }
    }
    
    py::tuple getNodePosition(ScenePtr scene, std::string node){
        SceneNodePtr n = scene->getNodeGlobal(node);
        if(n){        
            glm::mat4 trans = n->getAbsoluteTransform();
            return py::make_tuple(trans[3][0], trans[3][1], trans[3][2]);
        }
    }

    BOOST_PYTHON_MODULE(canis){
        py::class_<Scene>("Scene", py::no_init)
            .def("get_name", &Scene::getName)
        ;
        py::register_ptr_to_python<ScenePtr>();
        
        py::class_<Renderable>("Renderable", py::no_init);
        py::register_ptr_to_python<RenderablePtr>();
        
        py::class_<std::vector<RenderablePtr>>("RenderableList")
            .def(py::vector_indexing_suite<std::vector<RenderablePtr>>())
        ;
        
        py::def("dynamics_enabled", &dynamicsEnabled);
        py::def("get_active_scene", &getActiveScene/*, py::return_value_policy<py::reference_existing_object>()*/);
        
        py::def("load_mesh", &loadMesh);
        py::def("draw_mesh", &drawMesh);
        py::def("enqueue_mesh", &enqueueMesh);
        py::def("dequeue_mesh", &dequeueMesh);
        py::def("update_mesh_transform", &updateMeshTransform);
        
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
