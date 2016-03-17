#include "RenderGroup.h"
#include "Renderable.h"
#include "Renderer.h"

namespace Canis
{
    
    RenderGroup::RenderGroup(){
        
    }
    
    RenderGroup::~RenderGroup(){
        
    }
    
    size_t RenderGroup::enqueueRenderable(RenderablePtr renderable){
        if(_items.count(renderable->getName()) > 0){
            size_t n = _items[renderable->getName()]->count;

            const float* trans = glm::value_ptr(renderable->getTransform());
            memcpy(&_items[renderable->getName()]->transArray[n*16], &trans[0], sizeof(float)*16);

            /*const float* lightPos = glm::value_ptr(renderable->getLightPositions());
            memcpy(&_items[renderable->getName()]->lightPositionArray[n*16], &lightPos[0], sizeof(float)*16);
            
            const float* lightCol = glm::value_ptr(renderable->getLightColors());
            memcpy(&_items[renderable->getName()]->lightColorArray[n*16], &lightCol[0], sizeof(float)*16);    
            
            const float* lightRad = glm::value_ptr(renderable->getLightRadii());
            memcpy(&_items[renderable->getName()]->lightRadiiArray[n*4], &lightRad[0], sizeof(float)*4);  */            

            _items[renderable->getName()]->count += 1;
            
            renderable->_queueOffset = n;
            return n;
        }
        else{
            QueueItemPtr qi = std::make_shared<QueueItem>();
            qi->item = renderable->getVertexObjects();
            
            const float* trans = glm::value_ptr(renderable->getTransform());
            memcpy(&qi->transArray[0], &trans[0], sizeof(float)*16); 

            /*const float* lightPos = glm::value_ptr(renderable->getLightPositions());
            memcpy(&qi->lightPositionArray[0], &lightPos[0], sizeof(float)*16);
            
            const float* lightCol = glm::value_ptr(renderable->getLightColors());
            memcpy(&qi->lightColorArray[0], &lightCol[0], sizeof(float)*16);  
            
            const float* lightRad = glm::value_ptr(renderable->getLightRadii());
            memcpy(&qi->lightRadiiArray[0], &lightRad[0], sizeof(float)*4); */                      
            
            qi->count = 1;

            
            _items[renderable->getName()] = qi;
            
            renderable->_queueOffset = 0;
            return 0;
        }
    }
    
    void RenderGroup::updateRenderable(RenderablePtr renderable){
        if(renderable->isQueued()){
            size_t n = renderable->_queueOffset;
    
            const float* trans = glm::value_ptr(renderable->getTransform());
            memcpy(&_items[renderable->getName()]->transArray[n*16], &trans[0], sizeof(float)*16);
    
            /*const float* lightPos = glm::value_ptr(renderable->getLightPositions());
            memcpy(&_items[renderable->getName()]->lightPositionArray[n*16], &lightPos[0], sizeof(float)*16);  
            
            const float* lightCol = glm::value_ptr(renderable->getLightColors());
            memcpy(&_items[renderable->getName()]->lightColorArray[n*16], &lightCol[0], sizeof(float)*16);     
            
            const float* lightRad = glm::value_ptr(renderable->getLightRadii());
            memcpy(&_items[renderable->getName()]->lightRadiiArray[n*4], &lightRad[0], sizeof(float)*4);*/                     
        }      
    }
    
    QueueItemMap RenderGroup::getQueueItemMap(){
        return _items;
    }
    
}