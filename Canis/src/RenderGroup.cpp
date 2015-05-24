#include "RenderGroup.h"
#include "Renderable.h"

namespace Canis
{
    
    RenderGroup::RenderGroup(){
        
    }
    
    RenderGroup::~RenderGroup(){
        
    }
    
    void RenderGroup::queueRenderable(RenderablePtr renderable){
        if(_items.count(renderable->getName()) > 0){
            //_items[renderable->getName()]->item->update(renderable);
            _items[renderable->getName()]->transforms.push_back(renderable->getTransform());
            _items[renderable->getName()]->count += 1;
        }
        else{
            QueueItemPtr qi = std::make_shared<QueueItem>();
            qi->item = renderable;
            qi->count = 1;
            
            qi->transforms.push_back(renderable->getTransform());
            
            _items[renderable->getName()] = qi;
        }
    }
    
    QueueItemMap RenderGroup::getQueueItemMap(){
        return _items;
    }
    
}