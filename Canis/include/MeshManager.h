#ifndef __MESHMANAGER_H
#define __MESHMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    //TODO: getAndLoadIfNotFound helper?
    class CSAPI MeshManager : public Singleton<MeshManager>{
    private:
        std::map<std::string, MeshPtr> _meshes;
    
    public:
        MeshManager();
        virtual ~MeshManager();
        
        void addMesh(MeshPtr mesh);
        void removeMesh(std::string name);
        
        bool meshExists(std::string name);
        
        MeshPtr getMesh(std::string name);
        std::vector<MeshPtr> getMeshes();
    };

}

#endif
