#ifndef __DYNAMICLIBRARY_H
#define __DYNAMICLIBRARY_H

#include "Common.h"

#ifdef CANIS_PLATFORM_WIN32
    #define DYNLIB_HANDLE HINSTANCE
    #define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, 0 ) // we can not use LOAD_WITH_ALTERED_SEARCH_PATH with relative paths
    #define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
    #define DYNLIB_UNLOAD( a ) FreeLibrary( a )
#elif defined(CANIS_PLATFORM_GNU)
    #define DYNLIB_HANDLE void*
    #define DYNLIB_LOAD(a) dlopen(a, RTLD_LAZY | RTLD_GLOBAL)
    #define DYNLIB_GETSYM(a, b) dlsym(a, b)
    #define DYNLIB_UNLOAD(a) dlclose(a)
#endif

namespace Canis
{

    class CSAPI DynamicLibrary{
    private:
        std::string _name;
        DYNLIB_HANDLE _handle;

    public:
        DynamicLibrary(const std::string& name);

        void load();
        void unload();

        void* getSymbol(const std::string& symbol);
    };

}

#endif
