#ifndef __ASSIMPLOADER_H
#define __ASSIMPLOADER_H

#include "Common.h"
#include "VertexObject.h"

namespace Canis
{

    class CSAPI AssimpLoader{
    private:
        bool _loaded;
        const aiScene* _scene;
        VertexData _data;
        IndexData _indices;

    public:
        //TODO: this constructors are a little ambiguous - consider using boost filesyste path for fileName
        AssimpLoader(std::string fileName);
        AssimpLoader(char* buffer);
        ~AssimpLoader();

        void load(std::string fileName);

        VertexData getData();
        IndexData getIndices();
    };

}

#endif
