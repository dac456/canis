#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>
#include "MainWindow.h"
#include "Viewport.h"
//#include <Canis.h>

namespace SceneEditor
{

    Viewport::Viewport(QWidget* parent) : GL3Widget(parent){
        _timer = new QTimer(this);
        connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
        
        _initialized = false;
		
		//_timer->start(0);
		
        /*makeCurrent();
		glewExperimental = GL_TRUE;
		glewInit();
		
		if(GLEW_VERSION_3_0) {
			std::cout << "Good stuff, OpenGL 3.0 is available!" << std::endl;
		}
		
		if(glewIsSupported("GL_VERSION_4_0")) {
			std::cout << "Look at you, Mr. Fancyman, OpenGL 4.0 is available as well!" << std::endl;
		}*/
        
        //std::cout << glGetString(GL_VERSION) << std::endl;

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

        _mode = VIEW_MODE;
        
 
        
        //Q_EMIT contextCreated();
    }

    Viewport::~Viewport(){
        delete _nodeMarker;
        delete _selectionBox;
        
        if(_cam != nullptr){
            delete _cam;
        }
    }

    void Viewport::initialize(){
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

        MainWindow* main = qobject_cast<MainWindow*>(QApplication::topLevelWidgets()[0]);
        //connect(this, SIGNAL(contextCreated()), main, SLOT(initialize()));
        connect(this, SIGNAL(sceneChanged()), main, SLOT(updateSceneGraphTree()));  
    }
    
    void Viewport::render(){
        makeCurrent();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5, 1.0f);
        
        setView(_viewType);
        if(underMouse())
            setFocus(Qt::MouseFocusReason);        
        
        if(_activeScene != nullptr && _cam != nullptr && _initialized && this->isVisible()){
            //_activeScene->setActiveCamera(_cam);
            _activeScene->render(_cam, _projMatrix);
            
            glDisable(GL_DEPTH_TEST);
            for(size_t i=0; i<_activeScene->getNodes().size(); i++){
                renderNodeMarker(_activeScene->getNodes()[i]);
            }
            glEnable(GL_DEPTH_TEST);            
        }
        
        swapBuffers();
    }
    
    void Viewport::resize(int w, int h){
        glViewport(0, 0, w, h);
        
        if(_viewType == 0){
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

    /*void Viewport::initialize(Canis::Scene* defaultScene){
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        _cam = new Canis::Camera("Editor", glm::vec3(0.0, 1.8, 0.0), glm::vec3(0.0, 1.8, -5.0));
        Canis::RenderManager::getSingleton().addCamera(_cam);
        Canis::RenderManager::getSingleton().setActiveCamera(_cam);
        _lastPerspPos = _cam->getPosition();
        _lastPerspOrient = _cam->getOrientation();

        _currentScene = defaultScene;
        Canis::RenderManager::getSingleton().addScene(_currentScene);

        if(_viewType == 0)
            _projMatrix = glm::perspective(45.0f, (float)this->width()/(float)this->height(), 0.1f, 10000.0f);
        else
            _projMatrix = glm::ortho(0.0f, (float)this->width(), (float)this->height(), 0.0f, 0.1f, 10000.0f);

        Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);

        //Load editor specific assets
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Marker.material")));
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Selection.material")));
        _nodeMarker = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/node.ms3d"));
        _selectionBox = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/selection_box.ms3d"));

        fprintf(stdout, "GL Version: %s\n", glGetString(GL_VERSION));
        fprintf(stdout, "Canis Version: %i.%i.%i '%s'\n", CS_MAJOR_REVISION, CS_MINOR_REVISION, CS_BUILD_REVISION, CS_CODENAME);

        _timer->start(0);

        _initialised = true;
    }

    void Viewport::render(){
        makeCurrent();

        setView(_viewType);
        if(underMouse())
            setFocus(Qt::MouseFocusReason);

        Canis::RenderManager::getSingleton().update();

        glDisable(GL_DEPTH_TEST);
        for(size_t i=0; i<_currentScene->getNodes().size(); i++){
            renderNodeMarker(_currentScene->getNodes()[i]);
        }
        glEnable(GL_DEPTH_TEST);

        swapBuffers();
    }

    void Viewport::resize(int w, int h){
        glViewport(0, 0, w, h);

        if(_viewType == 0)
            _projMatrix = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 10000.0f);
        else{
            _orthoOrgX = -(this->width()/2.0f);
            _orthoMaxX = this->width()/2.0f;
            _orthoOrgY = -(this->height()/2.0f);
            _orthoMaxY = this->height()/2.0f;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
            //Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);
        }

        if(_initialised){
            Canis::RenderManager::getSingleton().resize(w, h);
            Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);
        }
    }*/
    
    /*void Viewport::initializeGL(){
        makeCurrent();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        _cam = new Canis::Camera("Editor", glm::vec3(0.0, 1.8, 0.0), glm::vec3(0.0, 1.8, -5.0));
        Canis::RenderManager::getSingleton().addCamera(_cam);
        Canis::RenderManager::getSingleton().setActiveCamera(_cam);
        _lastPerspPos = _cam->getPosition();
        _lastPerspOrient = _cam->getOrientation();

        //_currentScene = defaultScene;
        //Canis::RenderManager::getSingleton().addScene(_currentScene);

        if(_viewType == 0)
            _projMatrix = glm::perspective(45.0f, (float)this->width()/(float)this->height(), 0.1f, 10000.0f);
        else
            _projMatrix = glm::ortho(0.0f, (float)this->width(), (float)this->height(), 0.0f, 0.1f, 10000.0f);

        Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);

        //Load editor specific assets
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Marker.material")));
        Canis::MaterialManager::getSingleton().addMaterial(new Canis::Material(new Canis::MaterialLoader("../Editor/Materials/Selection.material")));
        _nodeMarker = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/node.ms3d"));
        _selectionBox = new Canis::Mesh(new Canis::AssimpLoader("./Media/Editor/Models/selection_box.ms3d"));

        fprintf(stdout, "GL Version: %s\n", glGetString(GL_VERSION));
        fprintf(stdout, "Canis Version: %i.%i.%i '%s'\n", CS_MAJOR_REVISION, CS_MINOR_REVISION, CS_BUILD_REVISION, CS_CODENAME);

        //_timer->start(0);

        _initialised = true;
    }
    
    void Viewport::paintGL(){
        makeCurrent();
        //swapBuffers();
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setView(_viewType);
        if(underMouse())
            setFocus(Qt::MouseFocusReason);

        Canis::RenderManager::getSingleton().update();

        glDisable(GL_DEPTH_TEST);
        for(size_t i=0; i<_currentScene->getNodes().size(); i++){
            renderNodeMarker(_currentScene->getNodes()[i]);
        }
        glEnable(GL_DEPTH_TEST);

        swapBuffers();        
    }
    
    void Viewport::resizeGL(int w, int h){
        makeCurrent();
        glViewport(0, 0, w, h);
        paintGL();
        
        if(_viewType == 0)
            _projMatrix = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 10000.0f);
        else{
            _orthoOrgX = -(this->width()/2.0f);
            _orthoMaxX = this->width()/2.0f;
            _orthoOrgY = -(this->height()/2.0f);
            _orthoMaxY = this->height()/2.0f;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
            //Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);
        }

        if(_initialised){
            //Canis::RenderManager::getSingleton().resize(w, h);
            //Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);
        }        
    }*/
    
    void Viewport::setActiveScene(Canis::Scene* scene){
        _activeScene = scene;
    }
    
    Canis::Scene* Viewport::getActiveScene(){
        return _activeScene;
    }
    
    /*
     * Events
     */

    void Viewport::mouseMoveEvent(QMouseEvent* e){
        if(_viewType == 0){
            if(_mode == VIEW_MODE){
                int pX = e->globalPos().x();
                int pY = e->globalPos().y();

                rotateFirstPerson(pX, pY);
            }
        }
        else{
            if(e->buttons() & Qt::LeftButton){
                int pX = e->pos().x();
                int pY = static_cast<int>(abs(e->pos().y() - this->height())); //Flip our Y coordinate in screen space

                float pZ;
                glReadPixels(pX, pY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pZ);

                glm::vec3 pos = glm::unProject(glm::vec3(pX, pY, pZ), Canis::RenderManager::getSingleton().getViewMatrix(), _projMatrix, glm::vec4(0, 0, this->width(), this->height()));

                if(_viewType == 1){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                std::cout << selectedNode->getName() << std::endl;
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                oldTrans[3][0] = pos.x;
                                oldTrans[3][2] = pos.z;
                                selectedNode->setTransform(oldTrans);
                            }
                        }
                    }
                }
                else if(_viewType == 2){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                oldTrans[3][0] = pos.x;
                                oldTrans[3][1] = pos.y;
                                selectedNode->setTransform(oldTrans);
                            }
                        }
                    }
                }
                else if(_viewType == 3){
                    if(_mode == MOVE_MODE){
                        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
                            Canis::SceneNode* selectedNode = getNodeByName(_selectedObjectName.toStdString());

                            if(selectedNode != nullptr){
                                glm::mat4 oldTrans = selectedNode->getTransform();
                                oldTrans[3][2] = pos.z;
                                oldTrans[3][1] = pos.y;
                                selectedNode->setTransform(oldTrans);
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
        if(e->buttons() & Qt::RightButton){
            _lastX = e->pos().x();
            _lastY = e->pos().y();
        }
    }

    void Viewport::wheelEvent(QWheelEvent* e){
        if(_viewType != 0){
            _orthoScale -= (float)e->delta()/(_scrollDivisor*8.0f);
            if(_orthoScale <= 0.0f)
                _orthoScale = 0.1f;

            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
            Canis::RenderManager::getSingleton().setProjectionMatrix(_projMatrix);
        }
        else{
            float delta = (float)(e->delta()/_scrollDivisor);
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

        if(_viewType != 0){
            _projMatrix = glm::ortho((_orthoOrgX*_orthoScale), (_orthoMaxX*_orthoScale), (_orthoOrgY*_orthoScale), (_orthoMaxY*_orthoScale), -10000.0f, 10000.0f);
        }
    }

    void Viewport::keyReleaseEvent(QKeyEvent* e){
        if(e->key() == Qt::Key_Shift)
            _scrollDivisor = 32.0f;
    }
    
    /*
     * Helpers
     */

    void Viewport::setView(int type){
        if(type == 0){
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }
        else if(type == 1){ //top
            _cam->setPosition(glm::vec3(_orthoOffsetX, 0.0f, _orthoOffsetY));
            //_cam->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            _cam->setRotation(glm::radians(-90.0f), 0.0f, 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(type == 2){ //front
            _cam->setPosition(glm::vec3(_orthoOffsetX, -_orthoOffsetY, 0.0f));
            _cam->setRotation(0.0f, 0.0f, 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
        else if(type == 3){ //side
            _cam->setPosition(glm::vec3(0.0f, -_orthoOffsetY, _orthoOffsetX));
            _cam->setRotation(0.0f, glm::radians(90.0f), 0.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
        }
    }

    void Viewport::rotateFirstPerson(int x, int y){
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

    void Viewport::renderNodeMarker(Canis::SceneNode* node){
        glm::mat4 trans = node->getAbsoluteTransform();

        float markerScale;
        if(_viewType == 0){
            glm::vec3 nodePosition = glm::vec3(trans[3][0], trans[3][1], trans[3][2]);
            float dist = glm::length(nodePosition - _cam->getPosition());
            markerScale = glm::max(dist/32.0f, 16.0f);
        }
        else
            markerScale = 4.0f;

        _nodeMarker->setTransform(trans*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
        
        _nodeMarker->render(_projMatrix, _cam->getTransform(), std::vector<Canis::Light*>());

        if(_selectedObjectType.compare("node") == 0){
            if(node->getName().compare(_selectedObjectName.toStdString()) == 0){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                _selectionBox->setTransform(trans*glm::scale(glm::vec3(markerScale, markerScale, markerScale)));
                _selectionBox->render(_projMatrix, _cam->getTransform(), std::vector<Canis::Light*>());
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        else if(_selectedObjectType.compare("entity") == 0){
            //TODO: the original plan was to scale the selection box to the size of the entities aabb.
            // however, not all entities have a bounding box, or are renderable. thus, selection box can only be draw around an entity icon of some kind
        }

        for(size_t i=0; i<node->getChildren().size(); i++){
            renderNodeMarker(node->getChildren()[i]);
        }
    }

    Canis::SceneNode* Viewport::getNodeByName(std::string name){
        Canis::SceneNode* selectedNode = nullptr;
        /*for(size_t i=0; i<_currentScene->getNodes().size(); i++){
            if(_currentScene->getNodes()[i]->getName().compare(name) == 0){
                selectedNode = _currentScene->getNodes()[i];
                break;
            }
            else{
                selectedNode = getChildNodeByName(_currentScene->getNodes()[i], name);
                if(selectedNode != nullptr)
                    break;
            }
        }*/

        return selectedNode;
    }

    Canis::SceneNode* Viewport::getChildNodeByName(Canis::SceneNode* node, std::string name){
        for(size_t i=0; i<node->getChildren().size(); i++){
            if(node->getChildren()[i]->getName().compare(name) == 0){
                return node->getChildren()[i];
            }
            else{
                return getChildNodeByName(node->getChildren()[i], name);
            }
        }

        return nullptr;
    }
    
    /*
     * Slots
     */
     
    void Viewport::selectObject(QString name, QString type){
        _selectedObjectName = name;
        _selectedObjectType = type;
        
        //Should eventually select by calling node->select();
    }        

    void Viewport::updateView(QString item){
        if(item.compare("Perspective") == 0){
            _projMatrix = glm::perspective(glm::radians(45.0f), (float)this->width()/(float)this->height(), 0.1f, 10000.0f);
            _viewType = 0;

            _cam->setPosition(_lastPerspPos);
            _cam->setOrientation(_lastPerspOrient);
        }
        else{
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

        if(item.compare("Top") == 0){
            _viewType = 1;
        }
        else if(item.compare("Front") == 0){
            _viewType = 2;
        }
        else if(item.compare("Side") == 0){
            _viewType = 3;
        }
    }

    void Viewport::setInitialPoseButtonClicked(){
        if(!Canis::Engine::getSingleton().isDynamicsEnabled()){
            Canis::SceneNode* selectedNode = getNodeByName(_selectedObjectName.toStdString());

            if(selectedNode != nullptr){
                selectedNode->setInitialTransform(selectedNode->getTransform());
                selectedNode->reset();

                for(size_t j=0; j<selectedNode->getEntities().size(); j++){
                    selectedNode->getEntities()[j]->reset();
                }
            }
        }
    }
    
    void Viewport::addSceneNode(QString name){
        glm::vec3 pos = _cam->getPosition();
        _activeScene->addSceneNode(new Canis::SceneNode(name.toStdString(), glm::translate(pos))); 
        
        Q_EMIT sceneChanged();
    }       

}
