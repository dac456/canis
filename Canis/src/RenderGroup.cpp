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
            //_items[renderable->getName()]->transforms.push_back(renderable->getTransform());
            const float* arr = glm::value_ptr(renderable->getTransform());
            memcpy(&_items[renderable->getName()]->transArray[(_items[renderable->getName()]->count*16)], &arr[0], sizeof(float)*16);
            _items[renderable->getName()]->count += 1;
        }
        else{
            QueueItemPtr qi = std::make_shared<QueueItem>();
            qi->item = renderable;
            
            const float* arr = glm::value_ptr(renderable->getTransform());
            /*for(size_t i=0; i<16; i++){
                qi->transArray[i] = arr[i];
            }*/
            memcpy(&qi->transArray[0], &arr[0], sizeof(float)*16);      
            
            qi->count = 1;
            
            //qi->transforms.push_back(renderable->getTransform());
            
            
            _items[renderable->getName()] = qi;
        }
    }
    
    QueueItemMap RenderGroup::getQueueItemMap(){
        return _items;
    }
    
}