#ifndef __RENDERTARGET_H
#define __RENDERTARGET_H

#include "Common.h"

namespace Canis
{

    class CSAPI RenderTarget{
    private:
        std::string _name;
        Camera* _camera;
        FramebufferObject* _fbo;

    public:
        RenderTarget(std::string name, Camera* camera, FramebufferObject* fbo);
        ~RenderTarget();

        void set();
    };

}

#endif
