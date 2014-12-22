#ifndef __UTILITY_H
#define __UTILITY_H

#include "Common.h"

namespace Canis
{

    class CSAPI Utility{
    public:
        static void log(std::string message);
        static std::string findAndReplace(const std::string& str, const std::string& find, const std::string& replace);
        static std::vector<std::string> split(std::string str, const char delim);
    };

}

#endif
