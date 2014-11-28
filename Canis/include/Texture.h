#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "Common.h"

namespace Canis
{

    typedef enum{
        TEXTURE_RGBA_UBYTE,
        TEXTURE_DEPTH_FLOAT,
        TEXTURE_DEPTH_UBYTE
    } TEXTURE_GEN_FORMAT;

    class CSAPI Texture{
    private:
        std::string _name;
        GLuint _textureId;
        GLuint _unit;

        bool _isLoaded;

    public:
        Texture(std::string name, std::string file, GLuint textureUnit);
        Texture(std::string name, unsigned char* data, int size, GLuint textureUnit);
        Texture(std::string name, unsigned int width, unsigned int height, TEXTURE_GEN_FORMAT textureFormat, GLuint textureUnit);
        ~Texture();

        void use(GLuint shader);

        GLuint getId();
        std::string getName();

        friend class TextureManager;
    };

}

#endif
