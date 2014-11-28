#include "GlShader.h"
#include "ProgramLoader.h"

namespace Canis
{

    GlShader::GlShader(std::string name, std::string vert, std::string frag){
        GLenum err = glGetError();
        _name = name;

        std::string vString = readFromFile(vert);
        const GLchar* vertexSrc = vString.c_str();
        const GLint vertexLength = vString.size();
        _vertId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertId, 1, &vertexSrc, &vertexLength);
        glCompileShader(_vertId);


        std::string fString = readFromFile(frag);
        const GLchar* fragmentSrc = fString.c_str();
        const GLint fragmentLength = fString.size();
        _fragId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(_fragId, 1, &fragmentSrc, &fragmentLength);
        glCompileShader(_fragId);

        _progId = glCreateProgram();
        glAttachShader(_progId, _vertId);
        glAttachShader(_progId, _fragId);
        glBindFragDataLocation(_progId, 0, "cs_FragColor0");
        glLinkProgram(_progId);

        GLchar buffer[512];
        GLsizei llength = 0;
        glGetProgramInfoLog(_progId, 512, &llength, buffer);
        printf("GLSL Log: %s\n", buffer);

        err = glGetError();
        //if(err != GL_NO_ERROR){
        //  fprintf(stderr, "%s\n", gluErrorString(err));
        //}
    }

    GlShader::GlShader(ProgramLoader* pl){
        ProgramLoaderData pd = pl->getData();
        GLenum err = glGetError();
        _name = pd.first;
        printf("Shader: %s\n", _name.c_str());

        //TODO: move source read to ProgramLoader
        std::string vString = readFromFile(pd.second.first);
        const GLchar* vertexSrc = vString.c_str();
        const GLint vertexLength = vString.size();
        _vertId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(_vertId, 1, &vertexSrc, &vertexLength);
        glCompileShader(_vertId);


        std::string fString = readFromFile(pd.second.second);
        const GLchar* fragmentSrc = fString.c_str();
        const GLint fragmentLength = fString.size();
        _fragId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(_fragId, 1, &fragmentSrc, &fragmentLength);
        glCompileShader(_fragId);

        _progId = glCreateProgram();
        glAttachShader(_progId, _vertId);
        glAttachShader(_progId, _fragId);
        glBindFragDataLocation(_progId, 0, "cs_FragColor0");
        glLinkProgram(_progId);

        GLchar buffer[512];
        GLsizei llength = 0;
        glGetProgramInfoLog(_progId, 512, &llength, buffer);

        printf("GLSL Log: %s\n", buffer);

        err = glGetError();
        //if(err != GL_NO_ERROR){
        //  fprintf(stderr, "%s\n", gluErrorString(err));
        //}
    }

    std::string GlShader::readFromFile(std::string fileName){
        std::ifstream fin("./Media/Shaders/"+fileName, std::ios::in);

        std::string file;
        while(fin.good()){
            std::string line;
            std::getline(fin, line);
            file.append(line+"\n");
        }
        file.append("\n\0");
        fin.close();

        return file;

    }

    void GlShader::use(){
        glUseProgram(_progId);
    }

    void GlShader::setUniformMat3f(std::string name, glm::mat3 value){
        GLuint loc = glGetUniformLocation(_progId, name.c_str());
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GlShader::setUniformMat4f(std::string name, glm::mat4 value){
        GLuint loc = glGetUniformLocation(_progId, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GlShader::setUniformVec3f(std::string name, glm::vec3 value){
        GLuint loc = glGetUniformLocation(_progId, name.c_str());
        glUniform3f(loc, value.x, value.y, value.z);
    }

    void GlShader::setUniformVec4f(std::string name, glm::vec4 value){
        GLuint loc = glGetUniformLocation(_progId, name.c_str());
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }

    void GlShader::setUniform1i(std::string name, int value){
        GLuint loc = glGetUniformLocation(_progId, name.c_str());
        glUniform1i(loc, value);
    }

    size_t GlShader::getId(){
        return static_cast<size_t>(_progId);
    }

    void GlShader::setId(size_t id){
        _id = id;
    }

    std::string GlShader::getName(){
        return _name;
    }

    GLuint GlShader::getProgramId(){
        return _progId;
    }
}
