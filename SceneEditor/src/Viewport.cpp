#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>
#include <QShortcut>
#include "MainWindow.h"
#include "Viewport.h"
//#include <Canis.h>

#ifdef CANIS_PLATFORM_WIN32
#define M_PI 3.14159265358979323846
#endif

Canis::Script* scr;
namespace SceneEditor
{

    Viewport::Viewport(QWidget* parent) : GL3Widget(parent){
        _timer = new QTimer(this);
        connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
        
        _initialized = false;
        _renderer = new Canis::Renderer();

        setFocusPolicy(Qt::ClickFocus);
        
        _cam = nullptr;
        _activeScene = nullptr;

        _viewType = 0;
        _selectedObjectName = "";
        _selectedObjectType = "";

        _lastX = QCursor::pos().x();
        _lastY = QCursor::pos().y();
        _mouseLock = false;

        _orthoScale = 1.0f;
        _orthoOrgX = -(this->width()/2.0f);
        _orthoMaxX = this->width()/2.0f;
        _orthoOrgY = -(this->height()/2.0f);
        _orthoMaxY = this->height()/2.0f;
        _orthoOffsetX = 0.0f;
        _orthoOffsetY = 0.0f;

        _scrollDivisor = 32.0f;
        
        _xAxisEnabled = true;
        _yAxisEnabled = false;
        _zAxisEnabled = true;

        _mode = VIEW_MODE;
        
        //Set up shortcuts
        connect(new QShortcut(QKeySequence(Qt::Key_1), this), SIGNAL(activated()), this, SLOT(setViewPerspective()));
        connect(new QShortcut(QKeySequence(Qt::Key_2), this), SIGNAL(activated()), this, SLOT(setViewTop()));
        connect(new QShortcut(QKeySequence(Qt::Key_3), this), SIGNAL(activated()), this, SLOT(setViewFront()));
        connect(new QShortcut(QKeySequence(Qt::Key_4), this), SIGNAL(activated()), this, SLOT(setViewSide()));

        //Q_EMIT contextCreated();
        scr = new Canis::Script("test", "./Media/Scripts/test.py");
    }

    Viewport::~Viewport(){
        delete _nodeMarker;
        delete _selectionBox;
        
        if(_activeScene != nullptr){
            delete _activeScene;
        }
        
        if(_cam != nullptr){
            delete _cam;
        }
        
        delete scr;
        delete _renderer;
    }
    
    void Viewport::connectToMainWindow(MainWindow* main){
        //connect(this, SIGNAL(contextCreated()), main, SLOT(initialize()));
        connect(this, SIGNAL(sceneChanged()), main, SLOT(updateSceneGraphTree()));
        connect(this, SIGNAL(viewportChanged(int)), main, SLOT(viewportChanged(int)));  
        connect(this, SIGNAL(renderOnce()), main, SLOT(updateFpsCounter()));
        connect(this, SIGNAL(setPropertySheetNode(Canis::SceneNode*)), main, SLOT(setPropertySheetNode(Canis::SceneNode*)));
        connect(this, SIGNAL(setPropertySheetLight(Canis::Light*)), main, SLOT(setPropertySheetLight(Canis::Light*)));
        connect(this, SIGNAL(setPropertySheetEntity(Canis::IEntity*)), main, SLOT(setPropertySheetEntity(Canis::IEntity*)));
    }        

    /*
     * Rendering methods
     */
    
    void Viewport::render(){
        makeCurrent();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5, 1.0f);
        
        _setView(_viewType);
        if(underMouse()) setFocus(Qt::MouseFocusReason);        
        
        if(_activeScene != nullptr && _cam != nullptr && _initialized && this->isVisible()){
            _activeScene->render(_cam, _projMatrix);
            
            glDisable(GL_DEPTH_TEST);
            for(size_t i=0; i<_activeScene->getNodes().size(); i++){
                _renderNodeMarker(_activeScene->getNodes()[i]);
            }
            glEnable(GL_DEPTH_TEST);            
        }
        
        swapBuffers();
        
        Q_EMIT renderOnce();
        
        Canis::ScriptManager::getSingleton().runStep(scr);
    }
    
    void Viewport::resize(int w, int h){
        glViewport(0, 0, w, h);
        
        if(_viewType == PERSP){
            _projMatrix = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 10000.0f);
        }
        else{
            _orthoOrgX = -(this->width()/2.0f);
            _orthoMaxX = this->width()/2.0f;
            _orthoOrgY = -(this->height()/2.0f);
            _orthoMaxY = this->height()/2.0f;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }        
    }
    
    Canis::Scene* Viewport::getActiveScene(){
        return _activeScene;
    }
    
    /*
     * Events
     */

    void Viewport::mouseMoveEvent(QMouseEvent* e){
        if(_viewType == PERSP){
            int pX = e->pos().x();
            int pY = e->pos().y();
                 
            if(_mode == VIEW_MODE){
                _rotateFirstPerson(e->globalPos().x(), e->globalPos().y());
            }
            else if(_mode == MOVE_MODE){
                int pX = e->pos().x();
                int pY = static_cast<int>(abs(e->pos().y() - this->height())); //Flip our Y coordinate in screen space

                float pZ;
                glReadPixels(pX, pY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pZ);
                
                glm::vec3 pos = glm::unProject(glm::vec3(pX, pY, pZ), _cam->getTransform(), _projMatrix, glm::vec4(this->geometry().x(), this->geometry().y(), this->width(), this->height()));
                
                //Plane intersection
                glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
                glm::vec4 o = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                glm::vec4 qp = o - glm::vec4(pos, 1.0f);
                float t = glm::dot(up, qp) / glm::dot(up, up);
                
                glm::vec4 P = glm::vec4(pos, 1.0f) + (t * up);
                
                if(e->buttons() & Qt::LeftButton){
                    if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                        Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                        
                        if(selectedNode != nullptr){
                            glm::mat4 trans = selectedNode->getTransform();
                            if(_xAxisEnabled) trans[3][0] = P.x;
                            if((QApplication::keyboardModifiers() & Qt::ControlModifier) || _yAxisEnabled) trans[3][1] = P.y;
                            if(_zAxisEnabled) trans[3][2] = P.z;
                            selectedNode->setTransform(trans);
                        }
                    }                  
                }                                   
            }
            else if(_mode == ROTATE_MODE){
                float dX = (pX - _lastX) / 750.0f;
                float dY = (pY - _lastY) / 750.0f;
                
                if(e->buttons() & Qt::LeftButton){
                    if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                        Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                        
                        if(selectedNode != nullptr){
                            glm::mat4 t = selectedNode->getTransform();
                            if(_xAxisEnabled) t = glm::rotate(t, dY/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
                            if(_yAxisEnabled) t = glm::rotate(t, dX/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                            if(_zAxisEnabled) t = glm::rotate(t, dY/2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
                            selectedNode->setTransform(t);
                        }
                    }                  
                } 
            }
            else if(_mode == SCALE_MODE){
                float dX = (pX - _lastX) / 1500.0f;
                float dY = (pY - _lastY) / 1500.0f;
                
                if(e->buttons() & Qt::LeftButton){
                    if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                        Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                        
                        if(selectedNode != nullptr){
                            glm::vec3 currentScale = selectedNode->getScale();
                            if(_xAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(dX, 0.0f, 0.0f));
                            if(_yAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, -dY, 0.0f));
                            if(_zAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, 0.0f, dY));
                        }
                    }                  
                }                 
            }
        }
        else{
            if(e->buttons() & Qt::LeftButton){
                int pX = e->pos().x();
                int pY = static_cast<int>(abs(e->pos().y() - this->height())); //Flip our Y coordinate in screen space
                
                float dX = (pX - _lastX) / 750.0f;
                float dY = (pY - _lastY) / 750.0f;

                float pZ;
                glReadPixels(pX, pY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pZ);

                glm::vec3 pos = glm::unProject(glm::vec3(pX, pY, pZ), _cam->getTransform(), _projMatrix, glm::vec4(0, 0, this->width(), this->height()));

                if(_viewType == TOP){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                if(_xAxisEnabled) oldTrans[3][0] = pos.x;
                                if(_zAxisEnabled) oldTrans[3][2] = pos.z;
                                selectedNode->setTransform(oldTrans);
                            }
                        }
                    }
                    else if(_mode == ROTATE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 t = selectedNode->getTransform();
                                if(_yAxisEnabled) t = glm::rotate(t, -dX/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                                selectedNode->setTransform(t);
                            }
                        }                        
                    }
                    else if(_mode == SCALE_MODE){
                        if(e->buttons() & Qt::LeftButton){
                            if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                                Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                                
                                if(selectedNode != nullptr){
                                    glm::vec3 currentScale = selectedNode->getScale();
                                    if(_xAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(dX, 0.0f, 0.0f));
                                    if(_zAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, 0.0f, -dY));
                                }
                            }                  
                        }                 
                    }                    
                }
                else if(_viewType == FRONT){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                if(_xAxisEnabled) oldTrans[3][0] = pos.x;
                                if(_yAxisEnabled) oldTrans[3][1] = pos.y;
                                selectedNode->setTransform(oldTrans);
                            }
                        }
                    }
                    else if(_mode == ROTATE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 t = selectedNode->getTransform();
                                if(_zAxisEnabled) t = glm::rotate(t, -dX/2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
                                selectedNode->setTransform(t);
                            }
                        }                        
                    }
                    else if(_mode == SCALE_MODE){
                        if(e->buttons() & Qt::LeftButton){
                            if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                                Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                                
                                if(selectedNode != nullptr){
                                    glm::vec3 currentScale = selectedNode->getScale();
                                    if(_xAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(dX, 0.0f, 0.0f));
                                    if(_yAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, dY, 0.0f));
                                }
                            }                  
                        }                 
                    }                                          
                }
                else if(_viewType == SIDE){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                if(_zAxisEnabled) oldTrans[3][2] = pos.z;
                                if(_yAxisEnabled) oldTrans[3][1] = pos.y;
                                selectedNode->setTransform(oldTrans);
                            }
                        }
                    }
                    else if(_mode == ROTATE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 t = selectedNode->getTransform();
                                if(_xAxisEnabled) t = glm::rotate(t, -dX/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
                                selectedNode->setTransform(t);
                            }
                        }                        
                    }
                    else if(_mode == SCALE_MODE){
                        if(e->buttons() & Qt::LeftButton){
                            if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                                Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());
                                
                                if(selectedNode != nullptr){
                                    glm::vec3 currentScale = selectedNode->getScale();
                                    if(_zAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, 0.0f, dX));
                                    if(_yAxisEnabled) selectedNode->setScale(selectedNode->getScale() + glm::vec3(0.0f, dY, 0.0f));
                                }
                            }                  
                        }                 
                    }                                          
                }
            }
            else if(e->buttons() & Qt::RightButton){
                int pX = e->pos().x();
                int pY = e->pos().y();

                int dX = pX - _lastX;
                int dY = pY - _lastY;

                if(dX < 0)
                    _orthoOffsetX -= abs(dX)/2;
                else if(dX > 0)
                    _orthoOffsetX += abs(dX)/2;

                if(dY < 0)
                    _orthoOffsetY -= abs(dY)/2;
                else if(dY > 0)
                    _orthoOffsetY += abs(dY)/2;

                _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);

                //_lastX = pX;
                //_lastY = pY;
            }
        }

        /*if(_viewType == 1){
            if(e->buttons() & Qt::LeftButton){
                if(_mode == MOVE_MODE){
                    int pX = e->pos().x();
                    int pY = e->pos().y();

                    float pZ;
                    glReadPixels(pX, pY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pZ);
                    //std::cout << pX << " " << pY << " " << pZ << std::endl;

                    glm::vec3 pos = glm::unProject(glm::vec3(pX, pY, pZ), Canis::RenderManager::getSingleton().getViewMatrix(), _projMatrix, glm::vec4(0, 0, this->width(), this->height()));
                    //std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;


                    int dX = pX - _lastX;
                    int dY = pY - _lastY;

                    for(size_t i=0; i<_currentScene->getNodes().size(); i++){
                        if(_currentScene->getNodes()[i]->getName().compare(_selectedObjectName.toStdString()) == 0){
                            glm::mat4 oldTrans = _currentScene->getNodes()[i]->getTransform();
                            oldTrans[3][0] = pos.x;
                            oldTrans[3][2] = -pos.z; //should be negative?
                            _currentScene->getNodes()[i]->setTransform(oldTrans);
                        }
                    }
                }
            }
        }*/
    }

    void Viewport::mousePressEvent(QMouseEvent* e){
        //if(e->buttons() & Qt::RightButton){
            _lastX = e->pos().x();
            _lastY = e->pos().y();
        //}
    }

    void Viewport::wheelEvent(QWheelEvent* e){
        if(_viewType != 0){
            _orthoScale -= (float)e->delta()/(_scrollDivisor*16.0f);
            if(_orthoScale <= 0.0f)
                _orthoScale = 0.1f;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }
        else{
            float delta = (float)(e->delta()/(_scrollDivisor*64.0f));
            _cam->move(glm::vec3(delta, delta, delta));
        }
    }

    void Viewport::keyPressEvent(QKeyEvent* e){
        if(e->key() == Qt::Key_Shift)
            _scrollDivisor = 8.0f;
        if(e->key() == Qt::Key_Q)
            _mode = VIEW_MODE;
        if(e->key() == Qt::Key_W)
            _mode = MOVE_MODE;
        if(e->key() == Qt::Key_R)
            _mode = ROTATE_MODE;
        if(e->key() == Qt::Key_S)
            _mode = SCALE_MODE;

        if(e->key() == Qt::Key_Left)
            if(_viewType != 0)
                _orthoOffsetX -= 32;

        if(e->key() == Qt::Key_Right)
            if(_viewType != 0)
                _orthoOffsetX += 32;

        if(e->key() == Qt::Key_Up)
            if(_viewType != 0)
                _orthoOffsetY -= 32;

        if(e->key() == Qt::Key_Down)
            if(_viewType != 0)
                _orthoOffsetY += 32;
                
        if(e->key() == Qt::Key_Delete){
            if(_selectedObjectType == "node"){
                _removeSceneNode(_selectedObjectName);
            }
            else if(_selectedObjectType == "entity"){
                _removeEntity(_selectedObjectName);
            }
            else if(_selectedObjectType == "light"){
                _removeLight(_selectedObjectName);
            }
        }

        if(_viewType != 0){
            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }
    }

    void Viewport::keyReleaseEvent(QKeyEvent* e){
        if(e->key() == Qt::Key_Shift){
            _scrollDivisor = 32.0f;
        }
    }
    
    /*
     * Helpers
     */

    void Viewport::_setView(int type){
        if(type == PERSP){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(type == TOP){ //top
            _cam->setPosition(glm::vec3(_orthoOffsetX, 0.0f, _orthoOffsetY));
            //_cam->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            _cam->setRotation(glm::radians(-90.0f), 0.0f, 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(type == FRONT){ //front
            _cam->setPosition(glm::vec3(_orthoOffsetX, -_orthoOffsetY, 0.0f));
            _cam->setRotation(0.0f, 0.0f, 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(type == SIDE){ //side
            _cam->setPosition(glm::vec3(0.0f, -_orthoOffsetY, _orthoOffsetX));
            _cam->setRotation(0.0f, glm::radians(90.0f), 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
    }
    
    void Viewport::_setOrtho(){
        float dx = _orthoMaxX - _orthoOrgX;
        float dy = _orthoMaxY - _orthoOrgY;

        if(dx >= dy){
            float dy2 = dx*this->height()/this->width();
            _orthoMaxY = _orthoOrgY + dy2;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }
        else{
            float dx2 = dy*this->width()/this->height();
            _orthoMaxX = _orthoOrgX + dx2;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }

        _lastPerspPos = _cam->getPosition();
        _lastPerspOrient = _cam->getOrientation();
    }        

    void Viewport::_rotateFirstPerson(int x, int y){
        int pX = x;
        int pY = y;

        int halfWidth = (this->geometry().left()+220+this->width())/2;
        int halfHeight = (this->geometry().top()+60+this->height())/2;

        //if((pX == halfWidth) && (pY == halfHeight))
        //  return;

        float dX = ((float)halfWidth - (float)pX) / 100.0f;
        float dY = ((float)halfHeight - (float)pY) / 100.0f;

        glm::vec3 angles;
        angles.x += dX;
        angles.y += dY;

        if(angles.x < -M_PI)
            angles.x += M_PI * 2;
        else if(angles.x > M_PI)
            angles.x -= M_PI * 2;

        if(angles.y < -M_PI / 2)
            angles.y = -M_PI / 2;
        if(angles.y > M_PI / 2)
            angles.y = M_PI / 2;

        //std::cout << angles.x << " " << angles.y << std::endl;
        _cam->rotate(glm::radians(angles.y*4.0f), glm::radians(angles.x*4.0f), 0.0f);
        //std::cout << _cam->getView().x << " " << _cam->getView().y << " " << _cam->getView().z << std::endl;

        QCursor::setPos(halfWidth, halfHeight);

        _lastX = pX;
        _lastY = pY;
    }

    void Viewport::_renderNodeMarker(Canis::SceneNode* node){
        glm::mat4 trans = node->getAbsoluteTransform();

        float markerScale;
        if(_viewType == 0){
            glm::vec3 nodePosition = glm::vec3(trans[3][0], trans[3][1], trans[3][2]);
            float dist = glm::length(nodePosition - _cam->getPosition());
            markerScale = glm::min(dist/64.0f, 4.0f);
        }
        else
            markerScale = 4.0f;

        //glm::vec3 nodeScale = node->getScale();
        //_nodeMarker->setTransform(trans*glm::scale(glm::vec3(1.0f/nodeScale.x, 1.0f/nodeScale.y, 1.0f/nodeScale.z))*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
        _nodeMarker->setTransform(glm::translate(glm::vec3(trans[3][0], trans[3][1], trans[3][2]))*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
        
        _nodeMarker->render(_projMatrix, _cam->getTransform(), std::vector<Canis::Light*>());

        if(_selectedObjectType.compare("node") == 0){
            if(node->getName().compare(_selectedObjectName.toStdString()) == 0){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                //_selectionBox->setTransform(trans*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
                _selectionBox->setTransform(glm::translate(glm::vec3(trans[3][0], trans[3][1], trans[3][2]))*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
                _selectionBox->render(_projMatrix, _cam->getTransform(), std::vector<Canis::Light*>());
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        else if(_selectedObjectType.compare("entity") == 0){
            //TODO: the original plan was to scale the selection box to the size of the entities aabb.
            // however, not all entities have a bounding box, or are renderable. thus, selection box can only be draw around an entity icon of some kind
        }

        for(size_t i=0; i<node->getChildren().size(); i++){
            _renderNodeMarker(node->getChildren()[i]);
        }
    }

    Canis::SceneNode* Viewport::_getNodeByName(std::string name){
        Canis::SceneNode* selectedNode = nullptr;
        for(size_t i=0; i<_activeScene->getNodes().size(); i++){
            if(_activeScene->getNodes()[i]->getName().compare(name) == 0){
                selectedNode = _activeScene->getNodes()[i];
                break;
            }
            else{
                selectedNode = _getChildNodeByName(_activeScene->getNodes()[i], name);
                if(selectedNode != nullptr)
                    break;
            }
        }

        return selectedNode;
    }

    Canis::SceneNode* Viewport::_getChildNodeByName(Canis::SceneNode* node, std::string name){
        for(size_t i=0; i<node->getChildren().size(); i++){
            if(node->getChildren()[i]->getName().compare(name) == 0){
                return node->getChildren()[i];
            }
            else{
                return _getChildNodeByName(node->getChildren()[i], name);
            }
        }

        return nullptr;
    }
    
    Canis::Light* Viewport::_getLightByName(QString name){
        Canis::Scene* sc = _activeScene;
        
        Canis::Light* ret = nullptr;
        for(auto n : sc->getNodes()){
            ret =  _getLightFromNode(n, name);
            
            if(ret != nullptr){
                return ret;
            }
            else{
                continue;
            }
        }
        
        return nullptr;
    }
    
    Canis::Light* Viewport::_getLightFromNode(Canis::SceneNode* node, QString name){
        for(auto l : node->getLights()){
            if(l->getName() == name.toStdString()){
                return l;
            }
        }
        
        for(auto c : node->getChildren()){
            return _getLightFromNode(c, name);
        }
        
        return nullptr;
    }
    
    Canis::IEntity* Viewport::_getEntityByName(QString name){
        Canis::Scene* sc = _activeScene;
        
        Canis::IEntity* ret = nullptr;
        for(auto n : sc->getNodes()){
            ret = _getEntityFromNode(n, name);
            
            if(ret != nullptr){
                return ret;
            }
            else{
                continue;
            }
        }
        
        return nullptr;
    }
    
    Canis::IEntity* Viewport::_getEntityFromNode(Canis::SceneNode* node, QString name){
        for(auto e : node->getEntities()){
            if(e->getName() == name.toStdString()){
                return e;
            }
        }
        
        for(auto c : node->getChildren()){
            return _getEntityFromNode(c, name);
        }
        
        return nullptr;
    }
    
    void Viewport::_removeSceneNode(QString name){
        Canis::SceneNode* node = _getNodeByName(name.toStdString());
        _activeScene->removeSceneNode(node);
        delete node;
        
        Q_EMIT sceneChanged();
    }
    
    void Viewport::_removeEntity(QString name){
        Canis::IEntity* ent = _getEntityByName(name);
        for(auto n : _activeScene->getNodes()){
            n->removeEntity(ent); //TODO: this should return if the entity was found/removed so we can break the loop
        }
        delete ent;
        
        Q_EMIT sceneChanged();
    }
    
    void Viewport::_removeLight(QString name){
        Canis::Light* light = _getLightByName(name);
        for(auto n : _activeScene->getNodes()){
            n->removeLight(light);
        }
        delete light;
        
        Q_EMIT sceneChanged();
    }
    
    /*
     * Slots
     */
     
    void Viewport::initialize(){
		//createContext(false);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        _cam = new Canis::Camera("Editor", glm::vec3(0.0, 1.8, 0.0), glm::vec3(0.0, 1.8, -5.0));
        
        if(_viewType == 0)
            _projMatrix = glm::perspective(45.0f, (float)this->width()/(float)this->height(), 0.1f, 10000.0f);
        else
            _projMatrix = glm::ortho(0.0f, (float)this->width(), (float)this->height(), 0.0f, 0.1f, 10000.0f);
            
        //Load editor specific assets
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Marker.material")));
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Selection.material")));
        _nodeMarker = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/node.ms3d"));
        _selectionBox = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/selection_box.ms3d"));                    
        
        fprintf(stdout, "GL Version: %s\n", glGetString(GL_VERSION));
        fprintf(stdout, "Canis Version: %i.%i.%i '%s'\n", CS_MAJOR_REVISION, CS_MINOR_REVISION, CS_BUILD_REVISION, CS_CODENAME);
        
        _timer->start(0);
        _initialized = true;
        
        Canis::Engine::getSingleton().setRenderer(_renderer);
    }     
     
    void Viewport::setActiveScene(Canis::Scene* scene){
        _activeScene = scene;
        //_renderer->addScene(scene);
        _renderer->setScene(scene);
    }     
     
    void Viewport::selectObject(QString name, QString type){
        _selectedObjectName = name;
        _selectedObjectType = type;
        
        //Should eventually select by calling node->select();
        
        if(type == "node"){
            Canis::SceneNode* n = _getNodeByName(name.toStdString());
            Q_EMIT setPropertySheetNode(n);
        }
        else if(type == "light"){
            Canis::Light* l = _getLightByName(name);
            Q_EMIT setPropertySheetLight(l);
        }
        else if(type == "entity"){
            Canis::IEntity* e = _getEntityByName(name);
            Q_EMIT setPropertySheetEntity(e);
        }
    }        

    void Viewport::updateView(QString item){
        if(item.compare("Perspective") == 0){
            setViewPerspective();
        }

        if(item.compare("Top") == 0){
            setViewTop();
        }
        else if(item.compare("Front") == 0){
            setViewFront();
        }
        else if(item.compare("Side") == 0){
            setViewSide();
        }
    }

    void Viewport::setInitialPoseButtonClicked(){
        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
            Canis::SceneNode* selectedNode = _getNodeByName(_selectedObjectName.toStdString());

            if(selectedNode != nullptr){
                selectedNode->setInitialTransform(selectedNode->getTransform());
                //selectedNode->reset();
                Canis::Engine::getSingleton().reset();

                //for(size_t j=0; j<selectedNode->getEntities().size(); j++){
                //    selectedNode->getEntities()[j]->reset();
                //}
            }
        }
    }
    
    void Viewport::setViewPerspective(){
        _projMatrix = glm::perspective(glm::radians(45.0f), (float)this->width()/(float)this->height(), 0.1f, 10000.0f);
        _viewType = PERSP;
        Q_EMIT viewportChanged(_viewType);

        _cam->setPosition(_lastPerspPos);
        _cam->setOrientation(_lastPerspOrient);
    }
    
    void Viewport::setViewTop(){
        _setOrtho();
        _viewType = TOP;
        Q_EMIT viewportChanged(_viewType);
    }
    
    void Viewport::setViewFront(){
        _setOrtho();
        _viewType = FRONT;
        Q_EMIT viewportChanged(_viewType);
    }
    
    void Viewport::setViewSide(){
        _setOrtho();
        _viewType = SIDE;
        Q_EMIT viewportChanged(_viewType);
    }
    
    void Viewport::setXAxisEnabled(int enabled){
        _xAxisEnabled = enabled;
    }
    
    void Viewport::setYAxisEnabled(int enabled){
        _yAxisEnabled = enabled;
    }
    
    void Viewport::setZAxisEnabled(int enabled){
        _zAxisEnabled = enabled;
    }        
    
    void Viewport::addSceneNode(QString name){
        glm::vec3 pos = _cam->getPosition();
        _activeScene->addSceneNode(new Canis::SceneNode(name.toStdString(), glm::translate(pos))); 
        
        Q_EMIT sceneChanged();
    }
    
    void Viewport::addEntity(QString type, Canis::StringMap args){
        if(_selectedObjectType == "node"){
            Canis::IEntity* ent = Canis::EntityManager::getSingleton().getEntityFactory(type.toStdString())->createEntity(args["name"], glm::mat4(1.0f), args);
            _getNodeByName(_selectedObjectName.toStdString())->attachEntity(ent);
            
            Q_EMIT sceneChanged();
        }
    }

    void Viewport::addLight(QString name, float radius, QColor diffuse){
        if(_selectedObjectType == "node"){
            _getNodeByName(_selectedObjectName.toStdString())->attachLight(new Canis::Light(name.toStdString(), glm::vec3(diffuse.redF(), diffuse.greenF(), diffuse.blueF()), radius, glm::mat4(1.0f)));
        
            Q_EMIT sceneChanged();
        }
    }         

}
