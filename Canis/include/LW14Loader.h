#ifndef __LW14LOADER_H
#define __LW14LOADER_H

#include "Common.h"
#include "VertexObject.h"

namespace Canis
{

#pragma pack(push, packing)
#pragma pack(1)
#define PACK_STRUCT

    struct LW14Header{
        unsigned short mapVersion;
        unsigned char mapFlags;
        long nameCount;
        long nameOffset;
        long objectCount;
        long objectOffset;
    } PACK_STRUCT;

    struct LW14NameTable{
        std::string* name;
    } PACK_STRUCT;

    struct LW14Object{
        int objectClass;
        long dataSize;
    } PACK_STRUCT;

    struct LW14Group{
        unsigned char flags;
        long index;
    } PACK_STRUCT;

    struct LW14Material{
        unsigned char flags;
        int groupName;
        int objectName;
        //int extensionName;
    } PACK_STRUCT;

    struct LW14Key{
        int keyName;
        int keyValue;
    } PACK_STRUCT;

    struct LW14Indice{
        unsigned char vertex;
        float textureCoords[2];
        float lightmapCoords[2];
    } PACK_STRUCT;

    struct LW14Face{
        unsigned char flags;
        float planeEquation[4];
        float texturePosition[2];
        float textureScale[2];
        float textureRotation[2];
        float uMap[4];
        float vMap[4];
        float luxelSize;
        long smoothGroupIndex;
        long materialIndex;
        long lightmapIndex;
        unsigned char indiceCount;
        LW14Indice* indices;
    } PACK_STRUCT;

    struct LW14Brush{
        unsigned char flags;
        long keyCount;
        LW14Key* keys;
        long groupIndex;
        long visgroupIndex;
        unsigned char colour[3];
        unsigned char vertexCount;
        glm::vec3* vertices;
        unsigned char faceCount;
        LW14Face* faces;
    } PACK_STRUCT;

    struct LW14LightMap{
        unsigned char flags;
        unsigned char resolution;
        long format;
        unsigned char* pixels;
        Texture* texture;
        unsigned int index;
    } PACK_STRUCT;

    struct LW14MeshRef{
        unsigned char flags;
        int objectName;
        int groupName;
        unsigned char limbCount;
    } PACK_STRUCT;

#pragma pack(pop, packing)
#undef PACK_STRUCT

    class CSAPI LW14Loader{
    private:
        float _scale;

        VertexData _vertices;
        int _currentGroup;
        int _currentFace;

        std::vector<LW14Group*> _groups;
        std::vector<LW14Material*> _materials;
        std::vector<LW14Brush*> _brushes;
        std::vector<LW14LightMap> _lightMaps;

        bool _isLoaded;

    public:
        LW14Loader();
        LW14Loader(std::string fileName);
        ~LW14Loader();

        void load(std::string fileName);

        VertexData getData();
        Texture* getLightmap(size_t index);
    };

}

#endif
