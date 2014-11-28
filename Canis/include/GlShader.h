#ifndef __GLSHADER_H
#define __GLSHADER_H

#include "Common.h"

namespace Canis
{
    class CSAPI GlShader{
    private:
        size_t _id;
        std::string _name;
        GLuint _vertId, _fragId, _progId;

    public:
        GlShader(std::string name, std::string vert, std::string frag);
        GlShader(ProgramLoader* pl);

        void use();

        void setUniformMat3f(std::string name, glm::mat3 value);
        void setUniformMat4f(std::string name, glm::mat4 value);
        void setUniformVec3f(std::string name, glm::vec3 value);
        void setUniformVec4f(std::string name, glm::vec4 value);
        void setUniform1i(std::string name, int value);

        size_t getId();
        std::string getName();
        GLuint getProgramId();

    private:
        std::string readFromFile(std::string fileName);

    protected:
        void setId(size_t id);

        friend class GlShaderManager;
    };
}

#endif
