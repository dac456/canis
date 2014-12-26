#ifndef __ISCRIPTPROVIDER_H
#define __ISCRIPTPROVIDER_H

#include "Common.h"

namespace Canis
{
    
    class CSAPI IScriptProvider{
    private:
        std::string _type;
        
    public:
        IScriptProvider(std::string type){
            _type = type;
        }
        
        virtual ~IScriptProvider(){}
        
        virtual void initialize() = 0;
        virtual void execute(Script* script) = 0;
        
        std::string getType(){
            return _type;
        }
    };
    
}

#endif
