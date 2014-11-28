#ifndef __IPLUGIN_H
#define __IPLUGIN_H

#include "Common.h"

namespace Canis
{

    class CSAPI IPlugin{
    private:
        std::string _name;
        std::string _version;

    public:
        IPlugin(std::string name, std::string version){
            _name = name;
            _version = version;
        }
        virtual ~IPlugin(){}

        virtual void install() = 0;
        virtual void initialise() = 0;
        virtual void shutdown() = 0;
        virtual void uninstall() = 0;

        std::string getName(){
            return _name;
        }

        std::string getVersion(){
            return _version;
        }
    };

}

#endif
