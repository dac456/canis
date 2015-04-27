#include <boost/filesystem.hpp>
#include "Script.h"

namespace Canis
{

    Script::Script(std::string name, fs::path file){
        _name = name;
        _id = -1;
        _owner = nullptr;
        _filePath = file;
        
        std::string ext = file.extension().string();
        
        if(ext == ".py"){
            _type = "python";
        }
        else{
            _type = "unknown";
        }
        
        std::ifstream fin(file.string(), std::ios::in);
        
        std::stringstream ss;
        if(fin.is_open()){
            std::string line;
            while(std::getline(fin, line)){
                ss << line << '\n';
            }
            fin.close();
        }
        
        _buffer = new char[ss.str().length()+1];
        strcpy(_buffer, ss.str().c_str());
    }
    
    Script::~Script(){
        if(_buffer){
            delete[] _buffer;
        }
    }
    
    void Script::setOwner(IObject* owner){
        _owner = owner;
    }
    
    IObject* Script::getOwner(){
        return _owner;
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
    
    fs::path Script::getFilePath(){
        return _filePath;
    }

}
