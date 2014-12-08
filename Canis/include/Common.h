#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
    #define CANIS_PLATFORM_WIN32

    #include <Windows.h>

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    //Disables export warnings for STL and GLM classes.
    //This should be harmless, especially in the usual case where the engine DLL and the host application are built
    //with the same compiler.
    #pragma warning( disable: 4251 )

    //Disables 'non dll interface' warning triggered by inheriting btMotionState. Not sure how to fix this one yet
    #pragma warning( disable: 4275 )

#elif defined(__GNUC__)
    #define CANIS_PLATFORM_GNU

    #include <dlfcn.h>
#endif

#ifdef CS_API_EXPORT
    #ifdef CANIS_PLATFORM_WIN32
        #define CSAPI __declspec(dllexport)
    #elif defined(CANIS_PLATFORM_GNU)
        #define CSAPI __attribute__ ((visibility ("default")))
    #endif
#else
    #ifdef CANIS_PLATFORM_WIN32
        #define CSAPI __declspec(dllimport)
    #elif defined(CANIS_PLATFORM_GNU)
        #define CSAPI
    #endif
#endif

#include <boost/algorithm/string.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include <GL/glew.h>
#ifdef CANIS_PLATFORM_WIN32
#include <GL/wglew.h>
#elif defined(CANIS_PLATFORM_GNU)
#include <GL/glxew.h>
#endif
#include <GL/gl.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "AxisAlignedBox.h"
#include "Utility.h"


//#define M_PI 3.14159f

#define CS_MAJOR_REVISION 0
#define CS_MINOR_REVISION 3
#define CS_BUILD_REVISION 10
#define CS_CODENAME "Kobayashi Maru"

namespace Canis
{
    typedef enum
    {
        BOX_SHAPE,
        SPHERE_SHAPE,
        CAPSULE_SHAPE
    } COLLISION_SHAPE;

    typedef GLfloat Real;
    typedef std::map<std::string, std::string> StringMap;

    struct Technique;
    struct Pass;

    class AssimpLoader;
    class AxisAlignedBox;
    class Camera;
    class CSXLoader;
    class DIFLoader;
    class DXSLoader;
    class DynamicLibrary;
    class Engine;
    class EntityManager;
    class FramebufferObject;
    class GlShader;
    class GlShaderManager;
    class IEntity;
    class IEntityFactory;
    class IObject;
    class IPlugin;
    class Light;
    class LW14Loader;
    class Material;
    class MaterialLoader;
    class MaterialManager;
    class Mesh;
    class PluginManager;
    class ProgramLoader;
    class Prop;
    class PropFactory;
    class Scene;
    class SceneNode;
    class StaticMesh;
    class StaticMeshFactory;
    class Renderer;
    class RenderManager;
    class RenderTarget;
    template <class T> class Singleton;
    class Texture;
    class TextureManager;
    class VertexObject;
}

#endif
