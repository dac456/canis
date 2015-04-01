#ifndef __PYTHONPROVIDER_H
#define __PYTHONPROVIDER_H

#include "Common.h"
#include "IScriptProvider.h"

#include <boost/python.hpp>

namespace Canis
{
    
    class PythonProvider : public IScriptProvider{
    public:
        PythonProvider();
        ~PythonProvider();
        
        void initialize();
        void execute(Script* script);
        
        void runStep(Script* script);
    
    private:
        boost::python::object _loadModule(std::string name, const char* buffer);
    };
    
}

#endif
