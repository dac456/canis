#include "DIFLoader.h"

namespace Canis
{

    DIFLoader::DIFLoader(std::string fileName){
        _isLoaded = false;
        load(fileName);
    }

    DIFLoader::~DIFLoader(){
    }

    void DIFLoader::load(std::string fileName){
        std::ifstream fin(fileName, std::ios::binary);

        unsigned int resourceVersion;
        fin.read(reinterpret_cast<char*>(&resourceVersion), sizeof(unsigned int));

        if(resourceVersion == 44){
            bool hasPreview = false;
            fin.read(reinterpret_cast<char*>(&hasPreview), sizeof(bool));
            //TODO: read PNG preview image

            unsigned int numDetailLevels;
            fin.read(reinterpret_cast<char*>(&numDetailLevels), sizeof(unsigned int));

            for(unsigned int i=0; i<numDetailLevels; i++){
                unsigned int interiorVersion;
                fin.read(reinterpret_cast<char*>(&interiorVersion), sizeof(unsigned int));

                if(interiorVersion == 14){
                    unsigned int detailLevel, minPixels;
                    fin.read(reinterpret_cast<char*>(&detailLevel), sizeof(unsigned int));
                    fin.read(reinterpret_cast<char*>(&minPixels), sizeof(unsigned int));
                }
            }
        }

        fin.close();
    }

    VertexData DIFLoader::getData(){
        return _vertices;
    }

}
