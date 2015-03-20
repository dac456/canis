#include "PythonProvider.h"
#include "Script.h"
#include "ScriptManager.h"
#include <boost/python.hpp>

namespace Canis
{
    
    PythonProvider::PythonProvider()
        : IScriptProvider("python"){
        ScriptManager::getSingleton().registerProvider(this);
    }
    
    PythonProvider::~PythonProvider(){
    }
    
    void PythonProvider::initialize(){
        Py_Initialize();
    }
    
    void PythonProvider::execute(Script* script){
        boost::python::exec(script->getBuffer().c_str(), boost::python::import("__main__").attr("__dict__"));
    }
    
}
