#ifndef __DIFLOADER_H
#define __DIFLOADER_H

#include "Common.h"
#include "VertexObject.h"

namespace Canis
{

    class CSAPI DIFLoader{
    private:
        VertexData _vertices;
        bool _isLoaded;

    public:
        DIFLoader(std::string fileName);
        ~DIFLoader();

        void load(std::string fileName);

        VertexData getData();
    };

}

#endif
