#include <boost/filesystem.hpp>
#include "GlShaderManager.h"
#include "GlShader.h"
#include "ProgramLoader.h"

using namespace boost;

namespace Canis
{

    GlShaderManager::GlShaderManager(){
        _lastAccessed = -1;
        _nextId = 0;

        if(filesystem::is_directory("./Media/Shaders/")){
            //addShader(new GlShader(new ProgramLoader("basic.program")));

            filesystem::directory_iterator iter("./Media/Shaders/");
            for(; iter != filesystem::directory_iterator(); iter++){
                if(strcmp(iter->path().extension().string().c_str(), ".program") == 0)
                    addShader(new GlShader(new ProgramLoader(iter->path().filename().string())));
            }
        }
        //GlShader* basic = new GlShader("basic", "basic.vert", "basic.frag");
        //addShader(basic);
    }

    GlShaderManager::~GlShaderManager(){
        for(size_t i=0; i<_shaders.size(); i++)
            delete _shaders[i];
    }

    size_t GlShaderManager::addShader(GlShader* shader){
        if(!exists(shader)){
            //shader->setId(_nextId);
            _shaders.push_back(shader);
            //_shaders.insert(_shaders.begin()+_nextId, shader);

            _lastAccessed = _nextId;
            _nextId++;
            return shader->getId();
        }
        else
            return -1;
    }

    GlShader* GlShaderManager::getShader(std::string name){
        if(_lastAccessed != -1){
            if(_shaders[_lastAccessed]->getName().compare(name) == 0){
                return _shaders[_lastAccessed];
            }
        }

        for(size_t i=0; i<_shaders.size(); i++){
            if(_shaders[i]->getName().compare(name) == 0){
                _lastAccessed = i;
                return _shaders[i];
            }
        }

        return nullptr;
    }

    bool GlShaderManager::exists(std::string name){
        if(_lastAccessed != -1)
            if(_shaders[_lastAccessed]->getName().compare(name) == 0)
                return true;

        for(size_t i=0; i<_shaders.size(); i++){
            if(_shaders[i]->getName().compare(name) == 0){
                _lastAccessed = i;
                return true;
            }
        }

        return false;
    }

    bool GlShaderManager::exists(GlShader* shader){
        if(_lastAccessed != -1)
            if(_shaders[_lastAccessed] == shader)
                return true;

        for(size_t i=0; i<_shaders.size(); i++){
            if(_shaders[i] == shader){
                _lastAccessed = i;
                return true;
            }
        }

        return false;
    }

}
