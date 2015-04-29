#ifndef __ASSIMPLOADER_H
#define __ASSIMPLOADER_H

#include "Common.h"
#include "VertexObject.h"

namespace fs = boost::filesystem;

namespace Canis
{

    class CSAPI AssimpLoader{
    private:
        bool _loaded;
        const aiScene* _scene;
        VertexData _data;
        IndexData _indices;

    public:
        AssimpLoader(fs::path path);
        AssimpLoader(const char* buffer);
        ~AssimpLoader();

        void load();

        VertexData getData();
        IndexData getIndices();
    };

}

#endif
