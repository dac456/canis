#ifndef __RENDERGROUP_H
#define __RENDERGROUP_H

#include "Common.h"

namespace Canis
{
    
    struct QueueItem{
        std::vector<VertexObject*> item;
        size_t count = 0;
        
        GLfloat transArray[16*32768];
        /*GLfloat lightPositionArray[16*32768];
        GLfloat lightColorArray[16*32768];
        GLfloat lightRadiiArray[4*32768];*/
    };
    
    typedef std::shared_ptr<QueueItem> QueueItemPtr;
    typedef std::map<std::string, QueueItemPtr> QueueItemMap;
    
    class CSAPI RenderGroup{
    private:
        QueueItemMap _items;
        
    public:
        RenderGroup();
        ~RenderGroup();
        
        size_t enqueueRenderable(RenderablePtr renderable);
        void updateRenderable(RenderablePtr renderable);
        
        QueueItemMap getQueueItemMap();
    };

}

#endif