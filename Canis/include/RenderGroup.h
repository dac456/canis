#ifndef __RENDERGROUP_H
#define __RENDERGROUP_H

#include "Common.h"

namespace Canis
{
    
    struct QueueItem{
        RenderablePtr item;
        size_t count = 0;
        
        std::vector<glm::mat4> transforms;
        GLfloat transArray[16*32768];
    };
    
    typedef std::shared_ptr<QueueItem> QueueItemPtr;
    typedef std::map<std::string, QueueItemPtr> QueueItemMap;
    
    class CSAPI RenderGroup{
    private:
        QueueItemMap _items;
        
    public:
        RenderGroup();
        ~RenderGroup();
        
        void queueRenderable(RenderablePtr renderable);
        
        QueueItemMap getQueueItemMap();
    };

}

#endif