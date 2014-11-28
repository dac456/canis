#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "Common.h"

namespace Canis
{

    struct Pass{
        std::string name;

        bool lightingPass;
        size_t numLights;

        glm::vec4 diffuse;
        std::vector<GLuint> textures;

        GlShader* shader;

        bool blend;
        int blendSrc, blendDst;
    };

    struct Technique{
        std::string name;
        std::vector<Pass> passes;
    };

    class CSAPI Material{
    private:
        size_t _id;
        std::string _name;

        std::vector<Technique> _techniques;
        size_t _currentTechnique;
        

    public:
        Material(){}
        Material(std::string name, Technique defaultTechnique);
        Material(MaterialLoader* matLoader);
        ~Material();

        void use();
        void addTechnique(Technique technique);
        bool setTechnique(std::string name);
        Technique getCurrentTechnique();

        size_t getId();
        std::string getName();
        void setName(std::string name);

        std::vector<Technique> getTechniques(); //Safe?

    private:
        void _setId(size_t id);

        friend class MaterialManager;
    };

}

#endif
