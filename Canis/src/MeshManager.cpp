#include "MeshManager.h"
#include "Mesh.h"
#include "AssimpLoader.h"

namespace Canis
{
    
    MeshManager::MeshManager(){
        char* cube = "hex 0 0 0 1";
        MeshPtr cubeMesh = std::make_shared<Mesh>("cube", new AssimpLoader(cube));
        cubeMesh->setMaterialOverride("white");
        
        _meshes["cube"] = cubeMesh;
    }
    
    MeshManager::~MeshManager(){
    }
    
    void MeshManager::addMesh(MeshPtr mesh){
        _meshes[mesh->getName()] = mesh;
    }
    
    void MeshManager::removeMesh(std::string name){
        _meshes.erase(name);
    }
    
    bool MeshManager::meshExists(std::string name){
        return _meshes.count(name) > 0;
    }
    
    MeshPtr MeshManager::getMesh(std::string name){
        try{
            return _meshes.at(name);
        }
        catch(const std::out_of_range& e){
            return nullptr;
        }
    }
    
    std::vector<MeshPtr> MeshManager::getMeshes(){
        std::vector<MeshPtr> meshes;
        std::transform( _meshes.begin(), _meshes.end(),
                   std::back_inserter(meshes),
                   boost::bind(&std::map<std::string, MeshPtr>::value_type::second,_1) );
                   
        return meshes;        
    }
    
}
