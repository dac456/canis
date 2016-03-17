#ifndef __RENDERABLE_H
#define __RENDERABLE_H

#include "Common.h"
#include "VertexObject.h"

namespace Canis
{
    
    class CSAPI Renderable{
    private:
        std::string _name;
        size_t _queueOffset;
        
        Material* _material;
        std::vector<VertexObject*> _vertexObjects;
        
        glm::mat4 _transform;
        glm::mat3 _normalMatrix;
        
        glm::mat4 _lightPositions;
        glm::mat4 _lightColors;
        glm::vec4 _lightRadii;
        
    public:
        Renderable(std::string name, Material* mat, std::vector<VertexObject*> objects);
        ~Renderable();
        
        //void update(RenderablePtr renderable);
        
        std::string getName();
        std::vector<VertexObject*> getVertexObjects();
        
        glm::mat4 getTransform();
        void setTransform(glm::mat4 transform);
        
        glm::mat3 getNormalMatrix();
        void setNormalMatrix(glm::mat3 normalMatrix);
        
        Material* getMaterial();
        //void setMaterial(MaterialPtr mat); //needed?
        
        
        glm::mat4 getLightPositions();
        void setLightPositions(glm::mat4 lightPositions);
        
        glm::mat4 getLightColors();
        void setLightColors(glm::mat4 lightColors);
        
        glm::vec4 getLightRadii();
        void setLightRadii(glm::vec4 lightRadii); 
        
        bool isQueued();      
        
        friend class RenderGroup; 
    };

}

#endif