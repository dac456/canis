#ifndef __MATERIALMANAGER_H
#define __MATERIALMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI MaterialManager : public Singleton<MaterialManager>{
    private:
        std::vector<Material*> _materials;
        std::vector<Material*>::iterator _materialIterator;
        size_t _lastAccessed;
        size_t _nextId;

    public:
        MaterialManager();
        virtual ~MaterialManager();

        size_t addMaterial(Material* mat);
        void removeMaterial(size_t id);

        Material* getMaterial(std::string name);
        Material* getMaterial(size_t id);

        bool exists(std::string name);
        size_t getNumMaterials();
    };

}

#endif
