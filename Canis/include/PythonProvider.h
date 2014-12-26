#ifndef __PYTHONPROVIDER_H
#define __PYTHONPROVIDER_H

#include "Common.h"
#include "IScriptProvider.h"

namespace Canis
{
    
    class PythonProvider : public IScriptProvider{
    public:
        PythonProvider();
        ~PythonProvider();
        
        void initialize();
        void execute(Script* script);
    };
    
}

#endif
