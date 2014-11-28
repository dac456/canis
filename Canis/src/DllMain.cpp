#ifdef _WIN32

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "assimp.lib")

#ifndef _DEBUG
#pragma comment(lib, "SOILr.lib")
#pragma comment(lib, "boost_filesystem-vc100-mt-1_47.lib")
#pragma comment(lib, "BulletCollision_vs2010.lib")
#pragma comment(lib, "BulletDynamics_vs2010.lib")
#pragma comment(lib, "LinearMath_vs2010.lib")
#else
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "boost_filesystem-vc100-mt-gd-1_47.lib")
#pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "LinearMath_vs2010_debug.lib")
#endif

#include "Common.h"

BOOL APIENTRY DllMain(
HANDLE hModule, // Handle to DLL module
    DWORD ul_reason_for_call,
    LPVOID lpReserved ) // Reserved
{
    /*switch ( ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACHED:
        // A process is loading the DLL.
        break;
        case DLL_THREAD_ATTACHED:
        // A process is creating a new thread.
        break;
        case DLL_THREAD_DETACH:
        // A thread exits normally.
        break;
        case DLL_PROCESS_DETACH:
        // A process unloads the DLL.
        break;
    }*/
    return TRUE;
}

#endif //_WIN32
