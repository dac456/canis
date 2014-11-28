#include "SkyBox.h"
#include "VertexObject.h"
#include "MaterialManager.h"
#include "MaterialLoader.h"
#include "Material.h"
#include "Mesh.h"
#include "LW14Loader.h"

namespace Canis
{

    float cube[100] =
     {-1.000000, 1.000000, 1.000000, 0.000000, 0.000000 ,
     -1.000000, -1.000000, 1.000000, 0.000000, 1.000000 ,
     1.000000, 1.000000, 1.000000, 1.000000, 0.000000 ,
     1.000000, -1.000000, 1.000000, 1.000000, 1.000000 ,
     1.000000, 1.000000, 1.000000, 0.000000, 0.000000 ,
     1.000000, -1.000000, 1.000000, 0.000000, 1.000000 ,
     1.000000, 1.000000, -1.000000, 1.000000, 0.000000 ,
     1.000000, -1.000000, -1.000000, 1.000000, 1.000000 ,
     1.000000, 1.000000, -1.000000, 0.000000, 0.000000 ,
     1.000000, -1.000000, -1.000000, 0.000000, 1.000000 ,
     -1.000000, 1.000000, -1.000000, 1.000000, 0.000000 ,
     -1.000000, -1.000000, -1.000000, 1.000000, 1.000000 ,
     -1.000000, 1.000000, -1.000000, 0.000000, 0.000000 ,
     -1.000000, -1.000000, -1.000000, 0.000000, 1.000000 ,
     -1.000000, 1.000000, 1.000000, 1.000000, 0.000000 ,
     -1.000000, -1.000000, 1.000000, 1.000000, 1.000000 ,
     -1.000000, 1.000000, 1.000000, 0.000000, 1.000000 ,
     1.000000, 1.000000, 1.000000, 1.000000, 1.000000 ,
     -1.000000, -1.000000, 1.000000, 0.000000, 0.000000, 
     1.000000, -1.000000, 1.000000, 1.000000, 0.000000 };

/*float vertices[72] =
{
    -1.0f, -1.0f, 1.0f,//VO - 0
    1.0f, -1.0f, 1.0f,//V1 - 1
    -1.0f, 1.0f, 1.0f,//V2 - 2
    1.0f, 1.0f, 1.0f,//V3 - 3

    -1.0f, -1.0f, -1.0f,//V4 - 4
    1.0f, -1.0f, -1.0f,//V5 - 5
    -1.0f, 1.0f, -1.0f,//V6 - 6
    1.0f, 1.0f, -1.0f,//V7 - 7

    -1.0f, 1.0f, 1.0f,//V2 - 8
    1.0f, 1.0f, 1.0f,//V3 - 9
    -1.0f, 1.0f, -1.0f,//V6 - 10
    1.0f, 1.0f, -1.0f,//V7 - 11

    -1.0f, -1.0f, 1.0f,//VO - 12
    1.0f, -1.0f, 1.0f,//V1 - 13
    -1.0f, -1.0f, -1.0f,//V4 - 14
    1.0f, -1.0f, -1.0f,//V5 - 15

    -1.0f, -1.0f, 1.0f,//VO - 16
    -1.0f, 1.0f, 1.0f,//V2 - 17
    -1.0f, -1.0f, -1.0f,//V4 - 18
    -1.0f, 1.0f, -1.0f,//V6 - 19

    1.0f, -1.0f, 1.0f,//V1 - 20
    1.0f, 1.0f, 1.0f,//V3 - 21
    1.0f, -1.0f, -1.0f,//V5 - 22
    1.0f, 1.0f, -1.0f,//V7 - 23
};*/

GLfloat     vertices[] =
  {
    -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
     1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
    -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
    -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
    -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
    -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
  };

GLubyte indices[36] =
{
    0,1,2, 1,3,2,
    6,7,5, 6,5,4,
    8,9,10, 9,10,11,
    12,13,14, 13,14,15,
    17,16,18, 17,18,19,
    20,21,22, 21,22,23
}; 

float textures[48] =
{
    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3

    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3

    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3

    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3

    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3

    0.0f, 1.0f,//0
    1.0f, 1.0f,//1
    0.0f, 0.0f,//2
    1.0f, 0.0f,//3
}; 


    SkyBox::SkyBox(std::string materialBaseName){
        //float size = FLT_MAX/8.0f;
        float size = 5.0f;

        //for(int i=0; i<73; i++)
        //  vertices[i] = vertices[i]*size;

        /*MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_left.material")));
        MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_right.material")));
        MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_front.material")));
        MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_back.material")));
        MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_top.material")));
        MaterialManager::getSingleton().addMaterial(new Material(new MaterialLoader(materialBaseName+"_bottom.material")));*/

        /*Vertex v[24];
        for(int i=0; i<24; i++){
            v[i].vertex[0] = vertices[(3*i)]*size;
            v[i].vertex[1] = vertices[1+(3*i)]*size;
            v[i].vertex[2] = vertices[2+(3*i)]*size;
            v[i].vertex[3] = 1.0f;
            v[i].normal[0] = v[i].normal[1] = v[i].normal[2] = v[i].normal[3] = 0.0f;
            v[i].texCoord[0] = textures[(2*i)];
            v[i].texCoord[1] = textures[1+(2*i)];
            fprintf(stdout, "%f, %f, %f\n", v[i].vertex[0], v[i].vertex[1], v[i].vertex[2]);
        };
        int mId = 0;
        for(int i=0; i<24; i+=4){
            Vertex tmp[4];
            tmp[0] = v[i];
            tmp[1] = v[i+1];
            tmp[2] = v[i+2];
            tmp[3] = v[i+3];
            VertexObject* o = new VertexObject(tmp, 4, GL_TRIANGLE_FAN, _materials[mId]->getId());
            _vertexObjects.push_back(o);
            mId++;
        }*/
        //_mesh = new Mesh(new LW14Loader("./Media/3DW/skybox-old.3dw"));
    }

    void SkyBox::render(){
        //for(size_t i=0; i<_vertexObjects.size(); i++)
        //  _vertexObjects[i]->render();
        //_mesh->render();
    }

}
