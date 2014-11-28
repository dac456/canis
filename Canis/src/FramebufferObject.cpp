#include "FramebufferObject.h"
#include "Texture.h"
#include "RenderManager.h"

namespace Canis
{

    FramebufferObject::FramebufferObject(unsigned int width, unsigned int height, std::vector<Texture*> colourAttachments, Texture* depthAttachment){
        _depthAttachment = nullptr;

        _width = width;
        _height = height;

        glGenFramebuffers(1, &_framebufferObject);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebufferObject);

        for(size_t i=0; i<colourAttachments.size(); i++){
            _colourAttachments.push_back(colourAttachments[i]);
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, colourAttachments[i]->getId(), 0);
        }

        if(depthAttachment != nullptr){
            _depthAttachment = depthAttachment;
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthAttachment->getId(), 0);
            /*glGenRenderbuffers(1, &_renderBufferDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBufferDepth);*/
        }

        GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
        if(e != GL_FRAMEBUFFER_COMPLETE)
            printf("FramebufferObject(): failed to create FBO\n");

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    FramebufferObject::~FramebufferObject(){
        /*for(size_t i=0; i<_colourAttachments.size(); i++)
            delete _colourAttachments[i];

        if(_depthAttachment != nullptr)
            delete _depthAttachment;*/
    }

    void FramebufferObject::use(){
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebufferObject);

        /*if(!_colourAttachments.empty()){
            GLuint* attachments = new GLuint[_colourAttachments.size()];
            for(size_t i=0; i<_colourAttachments.size(); i++)
                attachments[i] = GL_COLOR_ATTACHMENT0+i;

            glDrawBuffers(_colourAttachments.size(), attachments);
        }
        else
            glDrawBuffer(GL_NONE);*/

        glViewport(0, 0, _width, _height);
        //RenderManager::getSingleton().setProjectionMatrix(glm::perspective(
    }

}
