#include <QtCore/QtCore>
#include "MainWindow.h"
#include "IEntityDialogFactory.h"
#include "StringProperty.h"
#include "ColorProperty.h"
#include "Mat4Property.h"

namespace SceneEditor
{

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        //showMaximized();
        menuBar()->setNativeMenuBar(false);
        statusBar()->addWidget(new QLabel("FPS: "));
        
        _fpsLabel = new QLabel("");
        _fpsTimer.start();
        _fpsCounter.frameCount = 0;
        _fpsCounter.lastTime = _fpsTimer.elapsed();
        statusBar()->addWidget(_fpsLabel);

        ui.viewList->addItem("Perspective");
        ui.viewList->addItem("Top");
        ui.viewList->addItem("Front");
        ui.viewList->addItem("Side");
        
        //Connect actions
        connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(exit()));
        connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));        

        connect(ui.viewList, SIGNAL(activated(QString)), ui.viewport, SLOT(updateView(QString)));
        connect(ui.sceneGraphView, SIGNAL(itemSelectionChanged()), this, SLOT(sceneGraphSelectionChanged()));
        connect(ui.addNodeButton, SIGNAL(clicked()), this, SLOT(addNodeButtonClicked()));
        connect(ui.addLightButton, SIGNAL(clicked()), this, SLOT(addLightButtonClicked()));
        connect(ui.addEntityButton, SIGNAL(clicked()), this, SLOT(addEntityButtonClicked()));
        connect(ui.pauseSimulationButton, SIGNAL(clicked()), this, SLOT(pauseSimulationButtonClicked()));
        connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetSimulationButtonClicked()));
        connect(ui.setPoseButton, SIGNAL(clicked()), ui.viewport, SLOT(setInitialPoseButtonClicked()));
        
        connect(ui.xLockButton, SIGNAL(clicked()), this, SLOT(xLockButtonClicked()));
        connect(ui.yLockButton, SIGNAL(clicked()), this, SLOT(yLockButtonClicked()));
        connect(ui.zLockButton, SIGNAL(clicked()), this, SLOT(zLockButtonClicked()));
        connect(this, SIGNAL(enableXAxis(int)), ui.viewport, SLOT(setXAxisEnabled(int)));
        connect(this, SIGNAL(enableYAxis(int)), ui.viewport, SLOT(setYAxisEnabled(int)));
        connect(this, SIGNAL(enableZAxis(int)), ui.viewport, SLOT(setZAxisEnabled(int)));
        
        connect(this, SIGNAL(objectSelected(QString, QString)), ui.viewport, SLOT(selectObject(QString, QString)));
        connect(this, SIGNAL(sceneNodeAdded(QString)), ui.viewport, SLOT(addSceneNode(QString)));
        connect(this, SIGNAL(entityAdded(QString, Canis::StringMap)), ui.viewport, SLOT(addEntity(QString, Canis::StringMap)));
        connect(this, SIGNAL(lightAdded(QString, float, QColor)), ui.viewport, SLOT(addLight(QString, float, QColor)));
        connect(this, SIGNAL(initialized()), ui.viewport, SLOT(initialize()));
        connect(this, SIGNAL(sceneLoaded(Canis::Scene*)), ui.viewport, SLOT(setActiveScene(Canis::Scene*)));
        
        ui.viewport->connectToMainWindow(this);

        _entityDialogManager = new EntityDialogManager();
    }

    MainWindow::~MainWindow()
    {
        delete _engine;
        delete _entityDialogManager;
    }

    void MainWindow::initialize(){
		ui.viewport->createContext(false); //Ugly, but the only way this will work it seems for WGL

        _engine = new Canis::Engine();
        _engine->setDynamicsEnabled(false);
        _engine->setRenderer(new Canis::Renderer());
        
        ui.pauseSimulationButton->setText("Play");

        //---Build test scene programmatically---//
        /*Canis::StringMap args;
        args["mass"] = "50.0f";
        args["shape"] = std::to_string((long long)Canis::SPHERE_SHAPE);
        args["mesh"] = "./Media/Models/sphere.ms3d";
        Canis::IEntity* ent = Canis::EntityManager::getSingleton().getEntityFactory("Prop")->createEntity("Ball", glm::mat4(1.0f), args);

        args.clear();
        args["mesh"] = "./Media/Models/terrain2.ms3d";
        Canis::IEntity* ent2 = Canis::EntityManager::getSingleton().getEntityFactory("Static Mesh")->createEntity("Terrain", glm::mat4(1.0f), args);*/

        Canis::Scene* defaultScene = new Canis::Scene("untitled", glm::mat4(1.0f));

        Canis::SceneNode* root = new Canis::SceneNode("Root");
        //Canis::SceneNode* ball = new Canis::SceneNode("BallNode", glm::translate(glm::vec3(0.0f, 1000.0f, 0.0f)));
        defaultScene->addSceneNode(root);
        /*defaultScene->addSceneNode(ball);

        root->attachEntity(ent2);
        ball->attachEntity(ent);
        ball->attachSceneNode(new Canis::SceneNode("TestNode", glm::translate(glm::vec3(500.0f, 0.0f, 0.0f))));*/

        //Canis::Light* light = new Canis::Light("Light0", glm::vec3(1.0f, 1.0f, 1.0f), 10000.0f, glm::translate(glm::vec3(0.0f, 200.0f, 0.0f)));
        //root->attachLight(light);
        //---End Scene---//
        
        //ui.viewport->setActiveScene(defaultScene);
        Q_EMIT sceneLoaded(defaultScene);

        _rootItem = nullptr;
        updateSceneGraphTree();

        //Canis::PluginManager::getSingleton().loadPlugin("./Plugins/CanisPluginTest.dll"); //TODO: load all plugins by searching directory
        //loadPlugins();

        std::map<std::string, Canis::IEntityFactory*>::iterator it;
        std::map<std::string, Canis::IEntityFactory*> entityFactories = Canis::EntityManager::getSingleton().getEntityFactories();
        for(it = entityFactories.begin(); it != entityFactories.end(); it++)
            ui.entityList->addItem(QString(((*it).first).c_str()));
            
        std::cout << "mainwindow initialized" << std::endl;
            
        Q_EMIT initialized();
    }

    void MainWindow::loadPlugins(){
        QDir pluginDir = QCoreApplication::instance()->applicationDirPath();

#ifdef CANIS_PLATFORM_WIN32
        if (pluginDir.dirName().toLower() == "debug" || pluginDir.dirName().toLower() == "release")
            pluginDir.cdUp();
#endif
        pluginDir.cd("Plugins");

        QStringList pluginList = pluginDir.entryList(QDir::Files);
        for(size_t i=0; i<pluginList.size(); i++){
            QPluginLoader loader(pluginDir.absoluteFilePath(pluginList.at(i)));
            QObject* plugin = loader.instance();
            if(plugin){
                IEntityDialogFactory* entityDialogFactory = qobject_cast<IEntityDialogFactory*>(plugin);
                if(entityDialogFactory){
                    EntityDialogManager::getSingleton().addEntityDialogFactory(entityDialogFactory);
                }
            }
        }
    }
    
    /*
     * Events
     */
    
    /*
     * Helpers
     */

    //TODO: scene graph methods should possibly be moved to new widget class
    void MainWindow::_traverseNode(Canis::SceneNode* node, QTreeWidgetItem* root){
        for(size_t i=0; i<node->getEntities().size(); i++){
            Canis::IEntity* currentEntity = node->getEntities()[i];
            QTreeWidgetItem* ent = new QTreeWidgetItem(QStringList(std::string(currentEntity->getName()+" <Entity>").c_str()));
            root->addChild(ent);
        }

        for(size_t i=0; i<node->getLights().size(); i++){
            Canis::Light* currentLight = node->getLights()[i];
            QTreeWidgetItem* light = new QTreeWidgetItem(QStringList(std::string(currentLight->getName()+" <Light>").c_str()));
            root->addChild(light);
        }

        for(size_t i=0; i<node->getCameras().size(); i++){
            Canis::Camera* currentCamera = node->getCameras()[i];
            QTreeWidgetItem* cam = new QTreeWidgetItem(QStringList(std::string(currentCamera->getName()+" <Camera>").c_str()));
            root->addChild(cam);
        }

        for(size_t i=0; i<node->getChildren().size(); i++){
            Canis::SceneNode* nextNode = node->getChildren()[i];
            QTreeWidgetItem* nextRoot = new QTreeWidgetItem(QStringList(std::string(nextNode->getName()+" <Node>").c_str()));
            root->addChild(nextRoot);

            _traverseNode(nextNode, nextRoot);
        }
    }
    
    /*
     * Slots
     */
     
    void MainWindow::viewportChanged(int type){
        ui.viewList->setCurrentIndex(type);
    }

    void MainWindow::sceneGraphSelectionChanged(){
        if(!ui.sceneGraphView->selectedItems().empty()){
            QTreeWidgetItem* sel = ui.sceneGraphView->selectedItems()[0];

            if(sel->text(0).contains("<Node>")){
                ui.propertyBrowser->reset(); //should be signal
                
                ui.addEntityButton->setEnabled(true);
                ui.addLightButton->setEnabled(true);
                ui.addCameraButton->setEnabled(true);
                ui.setPoseButton->setEnabled(true);

                QString name = sel->text(0).remove(" <Node>");
                Q_EMIT objectSelected(name, "node");
            }
            else if(sel->text(0).contains("<Entity>")){
                ui.propertyBrowser->reset(); //should be signal
                
                ui.addEntityButton->setEnabled(false);
                ui.addLightButton->setEnabled(false);
                ui.addCameraButton->setEnabled(false);
                ui.setPoseButton->setEnabled(false);

                QString name = sel->text(0).remove(" <Entity>");
                Q_EMIT objectSelected(name, "entity");
            }
            else if(sel->text(0).contains("<Light>")){
                ui.propertyBrowser->reset(); //should be signal
                
                ui.addEntityButton->setEnabled(false);
                ui.addLightButton->setEnabled(false);
                ui.addCameraButton->setEnabled(false);
                ui.setPoseButton->setEnabled(false);

                QString name = sel->text(0).remove(" <Light>");
                Q_EMIT objectSelected(name, "light");                
            }
            else{
                ui.propertyBrowser->reset(); //should be signal
                
                ui.addEntityButton->setEnabled(false);
                ui.addLightButton->setEnabled(false);
                ui.addCameraButton->setEnabled(false);
                ui.setPoseButton->setEnabled(false);

                Q_EMIT objectSelected("", "");
            }
        }
    }
    
    void MainWindow::setPropertySheetNode(Canis::SceneNode* node){
        ui.propertyBrowser->addProperty(new StringProperty(QString("Name"), QString(node->getName().c_str()), false, [this, node](QVariant data){
            node->setName(data.toString().toStdString());
            updateSceneGraphTree();
        }));
        
        glm::mat4 trans = node->getTransform();
        QMatrix4x4 mat;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                mat(i,j) = trans[i][j];
            }
        }
        
        ui.propertyBrowser->addProperty(new Mat4Property("Transform", mat, [this, node](QVariant data){
            QMatrix4x4 mat = data.value<QMatrix4x4>();
            glm::mat4 trans;
            for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                    trans[i][j] = mat(i,j);
                }
            }
            
            node->setTransform(trans);            
        }));           
    }
    
    void MainWindow::setPropertySheetLight(Canis::Light* light){
        ui.propertyBrowser->addProperty(new StringProperty("Name", QString(light->getName().c_str()), false, [this, light](QVariant data){
            light->setName(data.toString().toStdString());
            updateSceneGraphTree();
        }));         
        
        glm::vec3 color = light->getDiffuse();
        ui.propertyBrowser->addProperty(new ColorProperty("Diffuse", QColor(color.x*255, color.y*255, color.z*255), [this, light](QVariant data){
            QColor diffuse = data.value<QColor>();
            light->setDiffuse(glm::vec3(diffuse.redF(), diffuse.greenF(), diffuse.blueF()));
        }));
        
        glm::mat4 trans = light->getTransform();
        QMatrix4x4 mat;
        mat.setToIdentity();
        ui.propertyBrowser->addProperty(new Mat4Property("Transform", mat, [this, light](QVariant data){
        }));        
    }
    
    void MainWindow::setPropertySheetEntity(Canis::IEntity* entity){
        ui.propertyBrowser->addProperty(new StringProperty("Name", QString(entity->getName().c_str()), false, [this, entity](QVariant data){
            entity->setName(data.toString().toStdString());
            updateSceneGraphTree();
        }));
        
        for(auto pair : entity->getParamTypes()){
            if(pair.second == Canis::PARAM_REAL){
                ui.propertyBrowser->addProperty(new StringProperty(QString(pair.first.c_str()), QString(entity->getParam(pair.first).c_str()), false, [pair, entity](QVariant data){
                    entity->setParam(pair.first, data.toString().toStdString());
                })); 
            }
            else if(pair.second == Canis::PARAM_PATH){
                 ui.propertyBrowser->addProperty(new StringProperty(QString(pair.first.c_str()), QString(entity->getParam(pair.first).c_str()), true, [pair, entity](QVariant data){
                })); 
            }               
        }                       
    }

    void MainWindow::addNodeButtonClicked(){
        if(addNodeDialog.exec() == QDialog::Accepted){
            Q_EMIT sceneNodeAdded(addNodeDialog.getNodeName());
            addNodeDialog.close();
        }
    }
    
    void MainWindow::addLightButtonClicked(){
        if(addLightDialog.exec() == QDialog::Accepted){
            Q_EMIT lightAdded(addLightDialog.getName(), addLightDialog.getRadius(), addLightDialog.getColor());
            addLightDialog.close();
        }
    }

    void MainWindow::addEntityButtonClicked(){
        QListWidgetItem* entity = ui.entityList->selectedItems()[0];

        AddEntityDialog addEntityDialog;
        addEntityDialog.setDialog(entity->text().toStdString());
        if(addEntityDialog.exec() == QDialog::Accepted){
            Q_EMIT entityAdded(entity->text(), addEntityDialog.getEntityParams());
            addEntityDialog.close();
        }
        else{
            addEntityDialog.close();
        }
    }

    void MainWindow::pauseSimulationButtonClicked(){
        if(ui.pauseSimulationButton->text() == "Pause"){
            Canis::Engine::getSingleton().setDynamicsEnabled(false);
            ui.pauseSimulationButton->setText("Play");
        }
        else{
            Canis::Engine::getSingleton().setDynamicsEnabled(true);
            ui.pauseSimulationButton->setText("Pause");
        }
    }

    void MainWindow::resetSimulationButtonClicked(){
        Canis::Engine::getSingleton().setDynamicsEnabled(false);
        Canis::Engine::getSingleton().reset();
        ui.pauseSimulationButton->setText("Play");
    }
    
    void MainWindow::xLockButtonClicked(){
        Q_EMIT enableXAxis(ui.xLockButton->isChecked());
    }
    
    void MainWindow::yLockButtonClicked(){
        Q_EMIT enableYAxis(ui.yLockButton->isChecked());
    }
    
    void MainWindow::zLockButtonClicked(){
        Q_EMIT enableZAxis(ui.zLockButton->isChecked());
    }
    
    //TODO: this can probably be faster; deleting the whole tree will be dangerous for large scenes
    void MainWindow::updateSceneGraphTree(){
        Canis::Scene* currentScene = ui.viewport->getActiveScene();
        ui.sceneGraphView->removeItemWidget(_rootItem, 1);

        if(_rootItem != nullptr)
            delete _rootItem;

        _rootItem = new QTreeWidgetItem(QStringList("untitled <Scene>"));
        ui.sceneGraphView->addTopLevelItem(_rootItem);

        for(size_t i=0; i<currentScene->getNodes().size(); i++){
            Canis::SceneNode* currentNode = currentScene->getNodes()[i];
            QTreeWidgetItem* node = new QTreeWidgetItem(QStringList(std::string(currentNode->getName()+" <Node>").c_str()));
            _rootItem->addChild(node);

            _traverseNode(currentNode, node);
        }

        ui.sceneGraphView->expandAll();
    } 
    
    void MainWindow::updateFpsCounter(){
        _fpsCounter.frameCount++;
        
        qint64 currentTime = _fpsTimer.elapsed();
        if(currentTime - _fpsCounter.lastTime > 1000){
            std::stringstream ss;
            ss << _fpsCounter.frameCount*1000.0/(currentTime - _fpsCounter.lastTime);
            _fpsLabel->setText(ss.str().c_str());
            _fpsCounter.lastTime = currentTime;
            _fpsCounter.frameCount = 0;
        }
    }   
    
    /*
     * Slots: Actions
     */
     void MainWindow::about(){
        aboutWindow.show();

        std::stringstream ss;
        ss << "Loaded engine plugins:" << std::endl;
        for(size_t i=0; i<Canis::PluginManager::getSingleton().getPlugins().size(); i++){
            Canis::IPlugin* plugin = Canis::PluginManager::getSingleton().getPlugins()[i];
            ss << plugin->getName() << " " << plugin->getVersion() << std::endl;
        }
        aboutWindow.update(QString(ss.str().c_str()));
    }
                 
     void MainWindow::exit(){
         QApplication::instance()->quit();
     }    

}
