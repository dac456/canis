#ifndef __FRAMEBUFFEROBJECT_H
#define __FRAMEBUFFEROBJECT_H

#include "Common.h"

namespace Canis
{
    
    class CSAPI FramebufferObject{
    private:
        GLuint _framebufferObject;
        GLuint _renderBufferDepth;

        unsigned int _width;
        unsigned int _height;

        std::vector<Texture*> _colourAttachments;
        Texture* _depthAttachment;

    public:
        FramebufferObject(unsigned int width, unsigned int height, std::vector<Texture*> colourAttachments, Texture* depthAttachment = nullptr);
        ~FramebufferObject();

        void use();
    };

}

#endif
