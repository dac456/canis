#ifndef __GLSHADERMANAGER_H
#define __GLSHADERMANAGER_H

#include "Common.h"
#include "Singleton.h"

namespace Canis
{

    class CSAPI GlShaderManager : public Singleton<GlShaderManager>{
    private:
        std::vector<GlShader*> _shaders;
        size_t _lastAccessed;
        size_t _nextId;

    public:
        GlShaderManager();
        ~GlShaderManager();

        size_t addShader(GlShader* shader);
        GlShader* getShader(std::string name);
        bool exists(std::string name);
        bool exists(GlShader* shader);
    };

}

#endif
