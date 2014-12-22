#include "Utility.h"

namespace Canis
{

    void Utility::log(std::string message){
        std::cout << message << std::endl;

        std::ofstream fout("./log.txt", std::ios::out);
        fout << message << std::endl;
        fout.close();
    }

    std::string Utility::findAndReplace(const std::string& str, const std::string& find, const std::string& replace){
        std::string ret = str;

        while(ret.find(find) != std::string::npos){
            size_t pos = ret.find(find);
            ret.replace(pos, find.length(), replace);
        }

        return ret;
    }

    std::vector<std::string> Utility::split(std::string str, const char delim){
        std::vector<std::string> ret;
        
        while(str.find_first_of(delim) != std::string::npos){
            std::string tok = str.substr(0, str.find_first_of(delim)+1);
            ret.push_back(tok);
            str = str.erase(0, str.find_first_of(delim)+1);
        }
        ret.push_back(str);
        
        return ret;
    }
}
