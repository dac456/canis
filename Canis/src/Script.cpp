#include <boost/filesystem.hpp>
#include "Script.h"

namespace Canis
{

    Script::Script(std::string name, std::string file){
        _name = name;
        _id = -1;
        
        boost::filesystem::path p(file);
        std::string ext = p.extension().string();
        
        if(ext == ".py"){
            _type = "python";
        }
        else{
            _type = "unknown";
        }
        
        std::ifstream fin(file, std::ios::in);
        
        fin.seekg(0, fin.end);
        int length = fin.tellg();
        fin.seekg(0, fin.beg);
        
        _buffer = new char[length];
        fin.read(_buffer, length);
        
        fin.close();
    }
    
    Script::~Script(){
        if(_buffer){
            delete[] _buffer;
        }
    }
    
    std::string Script::getName(){
        return _name;
    }
    
    std::string Script::getType(){
        return _type;
    }
    
    std::string Script::getBuffer(){
        return std::string(_buffer);
    }

}
