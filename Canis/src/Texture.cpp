#include "Texture.h"
#include "TextureManager.h"
#include <SOIL/SOIL.h>

namespace Canis
{

    Texture::Texture(std::string name, std::string file, GLuint textureUnit){
        GLenum err = glGetError();
        printf("Texture: %s\n", name.c_str());

        std::string path = std::string("./Media/Textures/")+file;

        _textureId = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y|SOIL_FLAG_MIPMAPS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

        _unit = textureUnit;
        _name = name;
    }

    Texture::Texture(std::string name, unsigned char* data, int size, GLuint textureUnit){
        _textureId = SOIL_create_OGL_texture(data, size, size, 3, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        float maxAnisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

        _unit = textureUnit;
        _name = name;
        delete [] data;
    }

    Texture::Texture(std::string name, unsigned int width, unsigned int height, TEXTURE_GEN_FORMAT textureFormat, GLuint textureUnit){
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        if(textureFormat == TEXTURE_RGBA_UBYTE){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         width,
                         height,
                         0, GL_RGBA, GL_UNSIGNED_BYTE,
                         NULL);
        }
        else if(textureFormat == TEXTURE_DEPTH_FLOAT){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                         width,
                         height,
                         0, GL_DEPTH_COMPONENT, GL_FLOAT,
                         NULL);
        }
        else if(textureFormat == TEXTURE_DEPTH_UBYTE){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                         width,
                         height,
                         0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE,
                         NULL);
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        _name = name;
        _unit = textureUnit;
    }

    Texture::~Texture(){
        glDeleteTextures(1, &_textureId);
    }

    void Texture::use(GLuint shader){
        glActiveTexture(GL_TEXTURE0 + _unit);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glUniform1i(glGetUniformLocation(shader, ("cs_Texture"+std::to_string((unsigned long long)_unit)).c_str()), _unit);
    }

    GLuint Texture::getId(){
        return _textureId;
    }

    std::string Texture::getName(){
        return _name;
    }

}
