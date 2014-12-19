#ifdef _WIN32

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
