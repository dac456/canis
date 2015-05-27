#include "RenderGroup.h"
#include "Renderable.h"

namespace Canis
{
    
    RenderGroup::RenderGroup(){
        
    }
    
    RenderGroup::~RenderGroup(){
        
    }
    
    void RenderGroup::enqueueRenderable(RenderablePtr renderable){
        if(_items.count(renderable->getName()) > 0){
            //_items[renderable->getName()]->item->update(renderable);
            //_items[renderable->getName()]->transforms.push_back(renderable->getTransform());
            size_t n = _items[renderable->getName()]->count;

            const float* trans = glm::value_ptr(renderable->getTransform());
            memcpy(&_items[renderable->getName()]->transArray[n*16], &trans[0], sizeof(float)*16);

            const float* lightPos = glm::value_ptr(renderable->getLightPositions());
            memcpy(&_items[renderable->getName()]->lightPositionArray[n*16], &lightPos[0], sizeof(float)*16);

            _items[renderable->getName()]->count += 1;
        }
        else{
            QueueItemPtr qi = std::make_shared<QueueItem>();
            qi->item = renderable;
            
            const float* trans = glm::value_ptr(renderable->getTransform());
            memcpy(&qi->transArray[0], &trans[0], sizeof(float)*16); 

            const float* lightPos = glm::value_ptr(renderable->getLightPositions());
            memcpy(&qi->lightPositionArray[0], &lightPos[0], sizeof(float)*16);
            
            qi->count = 1;
            
            //qi->transforms.push_back(renderable->getTransform());
            
            
            _items[renderable->getName()] = qi;
        }
    }
    
    QueueItemMap RenderGroup::getQueueItemMap(){
        return _items;
    }
    
}