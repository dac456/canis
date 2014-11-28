#include "TextureManager.h"
#include "Texture.h"

namespace Canis
{

    TextureManager::TextureManager(){
        _lastAccessed = 0;
    }

    TextureManager::~TextureManager(){
        for(size_t i=0; i<_textures.size(); i++)
            delete _textures[i];

        _textures.clear();
    }

    void TextureManager::addTexture(Texture* texture){
        _textures.push_back(texture);
    }

    Texture* TextureManager::getTexture(std::string name){
        if(!_textures.empty())
            if(_textures[_lastAccessed]->getName().compare(name) == 0)
                return _textures[_lastAccessed];

        for(size_t i=0; i<_textures.size(); i++){
            if(_textures[i]->getName().compare(name) == 0){
                _lastAccessed = i;
                return _textures[i];
            }
        }

        return nullptr;
    }

    Texture* TextureManager::getTexture(GLuint id){
        if(!_textures.empty())
            if(_textures[_lastAccessed]->getId() == id)
                return _textures[_lastAccessed];

        for(size_t i=0; i<_textures.size(); i++){
            if(_textures[i]->getId() == id){
                _lastAccessed = i;
                return _textures[i];
            }
        }

        return nullptr;
    }
    bool TextureManager::exists(std::string name){
        if(!_textures.empty())
            if(_textures[_lastAccessed]->getName().compare(name) == 0)
                return true;

        for(size_t i=0; i<_textures.size(); i++){
            if(_textures[i]->getName().compare(name) == 0){
                _lastAccessed = i;
                return true;
            }
        }

        return false;
    }
}
