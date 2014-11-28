#ifndef __TEXTUREMANAGER_H
#define __TEXTUREMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI TextureManager : public Singleton<TextureManager>{
    private:
        std::vector<Texture*> _textures;
        size_t _lastAccessed;

    public:
        TextureManager();
        ~TextureManager();

        void addTexture(Texture* texture);

        Texture* getTexture(std::string name);
        Texture* getTexture(GLuint id);

        bool exists(std::string name);
    };

}

#endif
