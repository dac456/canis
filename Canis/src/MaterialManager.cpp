#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "Material.h"

using namespace boost;

namespace Canis
{

    MaterialManager::MaterialManager(){
        _lastAccessed = -1;
        _nextId = 0;
        
        if(filesystem::is_directory("./Media/Materials/")){
            //addMaterial(new Material(new MaterialLoader("system.material")));

            filesystem::directory_iterator iter("./Media/Materials/");
            for(; iter != filesystem::directory_iterator(); iter++){
                if(strcmp(iter->path().extension().string().c_str(), ".material") == 0)
                    addMaterial(new Material(new MaterialLoader(iter->path().filename().string())));
            }
        }
    }

    MaterialManager::~MaterialManager(){
        for(size_t i=0; i<_materials.size(); i++)
            delete _materials[i];
    }

    size_t MaterialManager::addMaterial(Material* mat){
            //_nextId++;

            mat->_setId(_nextId);
            //_materials.push_back(mat);
            _materialIterator = _materials.begin();
            _materials.insert(_materialIterator+_nextId, mat);

            _lastAccessed = _nextId;
            _nextId++;
            //return _nextId;
            return _lastAccessed;
    }

    void MaterialManager::removeMaterial(size_t id){
        Material* mat = getMaterial(id);
        //_nextId = mat->getId()-1;
        _materials.erase(_materials.begin()+mat->getId());
        delete mat;
    }

    Material* MaterialManager::getMaterial(std::string name){
        if(_lastAccessed != -1)
            if(boost::iequals(_materials[_lastAccessed]->getName(), name))
                return _materials[_lastAccessed];

        for(size_t i=0; i<_materials.size(); i++){
            if(boost::iequals(_materials[i]->getName(), name)){
                _lastAccessed = i;
                return _materials[i];
            }
        }
        return nullptr;
    }
    Material* MaterialManager::getMaterial(size_t id){
        if(_lastAccessed != -1)
            if(_materials[_lastAccessed]->getId() == id)
                return _materials[_lastAccessed];

        for(size_t i=0; i<_materials.size(); i++){
            if(_materials[i]->getId() == id){
                _lastAccessed = i;
                return _materials[i];
            }
        }
        return nullptr;
    }

    bool MaterialManager::exists(std::string name){
        if(_lastAccessed != -1)
            if(_materials[_lastAccessed]->getName().compare(name) == 0)
                return true;

        for(size_t i=0; i<_materials.size(); i++){
            if(_materials[i]->getName().compare(name) == 0){
                _lastAccessed = i;
                return true;
            }
        }

        return false;
    }

    size_t MaterialManager::getNumMaterials(){
        return _materials.size();
    }

}
