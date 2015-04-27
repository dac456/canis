#ifndef __SCRIPT_H
#define __SCRIPT_H

#include "Common.h"

namespace fs = boost::filesystem;

namespace Canis
{
    
    class CSAPI Script{
    private:
        size_t _id;
        std::string _name;
        std::string _type;
        IObject* _owner;
        
        char* _buffer;
        fs::path _filePath;
        
    public:
        Script(std::string name, fs::path file);
        ~Script();
        
        void setOwner(IObject* owner);
        IObject* getOwner();
        
        std::string getName();
        std::string getType();
        std::string getBuffer();
        fs::path getFilePath();
        
        friend class ScriptManager;
    };
    
}

#endif
