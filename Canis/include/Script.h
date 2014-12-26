#ifndef __SCRIPT_H
#define __SCRIPT_H

#include "Common.h"

namespace Canis
{
    
    class CSAPI Script{
    private:
        size_t _id;
        std::string _name;
        std::string _type;
        
        char* _buffer;
        
    public:
        Script(std::string name, std::string file);
        ~Script();
        
        std::string getName();
        std::string getType();
        std::string getBuffer();
        
        friend class ScriptManager;
    };
    
}

#endif
