#include "DynamicLibrary.h"

namespace Canis
{

    DynamicLibrary::DynamicLibrary(const std::string& name){
        _name = name;
        _handle = NULL;
    }

    void DynamicLibrary::load(){
        std::string name = _name;

#ifdef CANIS_PLATFORM_WIN32
        if(name.substr(name.length()-4, 4) != ".dll")
            name += ".dll";
#endif

        _handle = (DYNLIB_HANDLE)DYNLIB_LOAD(name.c_str());

        if(!_handle)
            printf("DynamicLibrary::load(): error loading library.\n");
    }

    void DynamicLibrary::unload(){
        DYNLIB_UNLOAD(_handle);
    }

    void* DynamicLibrary::getSymbol(const std::string& symbol){
        return (void*)DYNLIB_GETSYM(_handle, symbol.c_str());
    }

}
