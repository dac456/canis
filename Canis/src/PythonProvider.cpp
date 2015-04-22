#include "PythonProvider.h"
#include "Script.h"
#include "ScriptManager.h"
#include <boost/filesystem.hpp>

#include "python/python_canis.h"

namespace py = boost::python;

namespace Canis
{
		
    PythonProvider::PythonProvider()
        : IScriptProvider("python"){
        ScriptManager::getSingleton().registerProvider(this);
    }
    
    PythonProvider::~PythonProvider(){
		//Py_Finalize();
    }
    
    void PythonProvider::initialize(){
        Py_Initialize();
        registerModuleSceneGraph();
        
        boost::filesystem::path p("./Media/Scripts");
        std::cout << boost::filesystem::absolute(p).normalize().string() << std::endl;
        py::import("sys").attr("path").attr("append")(boost::filesystem::absolute(p).normalize().string());
    }
    
    /*void PythonProvider::execute(Script* script){
		py::object mainModule = py::import("__main__");
		py::object mainNamespace = mainModule.attr("__dict__");
		
        py::exec(script->getBuffer().c_str(), mainNamespace, mainNamespace);
    }*/
    
    void PythonProvider::run(Script* script, std::string method){
		try{
			//TODO: store module if script->isModule() (or something)
			py::object module = _loadModule(script->getName(), script->getBuffer().c_str());
            py::dict base_dict = py::extract<py::dict>(module.attr("__dict__"));
            base_dict["this_object_name"] = py::str(script->getOwner()->getName().c_str());
            base_dict["this_object_type"] = py::str(script->getOwner()->getType().c_str());
			module.attr(method.c_str())();
		} catch(const py::error_already_set& e){
			/* http://strattonbrazil.blogspot.ca/2011/09/adding-python-support-using-boostpython.html */
			
			PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
			PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
			std::string ret("Unfetchable Python error");
			if (type_ptr != NULL) {
				py::handle<> h_type(type_ptr);
				py::str type_pstr(h_type);
				py::extract<std::string> e_type_pstr(type_pstr);
				if(e_type_pstr.check())
					ret = e_type_pstr();
				else
					ret = "Unknown exception type";
			}

			if (value_ptr != NULL) {
				py::handle<> h_val(value_ptr);
				py::str a(h_val);
				py::extract<std::string> returned(a);
				if(returned.check())
					ret +=  ": " + returned();
				else
					ret += std::string(": Unparseable Python error: ");
			}

			if (traceback_ptr != NULL) {
				py::handle<> h_tb(traceback_ptr);
				py::object tb(py::import("traceback"));
				py::object fmt_tb(tb.attr("format_tb"));
				py::object tb_list(fmt_tb(h_tb));
				py::object tb_str(py::str("\\n").join(tb_list));
				py::extract<std::string> returned(tb_str);
				if(returned.check())
					ret += ": " + returned();
				else
					ret += std::string(": Unparseable Python traceback");
			}
			
			std::cout << ret << std::endl;;			
		}
	}
	
	py::object PythonProvider::_loadModule(std::string name, const char* module_source)
	{
		/* http://stackoverflow.com/questions/26649995/attributeerror-module-object-has-no-attribute-func-only-with-c-invocation */

		// create module name and filenames
		py::object name_str(py::str(name.c_str()));
		std::string file = name;
		file += ".py";
		py::object file_str(py::str(file.c_str()));

		// get helper modules
		py::object types(py::import("types"));
		py::object sys(py::import("sys"));

		if (py::dict(sys.attr("modules")).has_key(name_str)) {
			return sys.attr("modules")[name_str];
		}

		// create empty module
		py::object module(types.attr("ModuleType")(name_str));

		// init module dict
		py::dict base_dict = py::extract<py::dict>(module.attr("__dict__"));
		base_dict["__builtins__"] = py::import("__builtin__");
		base_dict["__name__"] = name_str;
		base_dict["__file__"] = file_str;


		py::object canis_module( (py::handle<>(PyImport_ImportModule("canis"))) );
		base_dict["canis"] = canis_module;

		// execute module code in module context
		py::exec(module_source, base_dict, base_dict);

		// add module to sys.modules
		sys.attr("modules")[name] = module;

		return module;
	}	
    
}
