#include <QtCore/QtCore>
#include "MainWindow.h"
#include "IEntityDialogFactory.h"

namespace SceneEditor
{

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);

        showMaximized();
        menuBar()->setNativeMenuBar(false);

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
        connect(ui.addEntityButton, SIGNAL(clicked()), this, SLOT(addEntityButtonClicked()));
        connect(ui.pauseSimulationButton, SIGNAL(clicked()), this, SLOT(pauseSimulationButtonClicked()));
        connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetSimulationButtonClicked()));
        connect(ui.setPoseButton, SIGNAL(clicked()), ui.viewport, SLOT(setInitialPoseButtonClicked()));
        
        connect(this, SIGNAL(objectSelected(QString, QString)), ui.viewport, SLOT(selectObject(QString, QString)));
        connect(this, SIGNAL(sceneNodeAdded(QString)), ui.viewport, SLOT(addSceneNode(QString)));
        
        ui.viewport->connectToMainWindow(this);

        _entityDialogManager = new EntityDialogManager();
    }

    MainWindow::~MainWindow()
    {
        delete _engine;
        delete _entityDialogManager;
    }

    void MainWindow::initialize(){
        _engine = new Canis::Engine();
        _engine->setDynamicsEnabled(false);
        ui.pauseSimulationButton->setText("Play");

        //---Build test scene programmatically---//
        Canis::StringMap args;
        args["mass"] = "50.0f";
        args["shape"] = std::to_string((long long)Canis::SPHERE_SHAPE);
        args["mesh"] = "./Media/Models/sphere.ms3d";
        Canis::IEntity* ent = Canis::EntityManager::getSingleton().getEntityFactory("Prop")->createEntity("Ball", glm::mat4(1.0f), args);

        args.clear();
        args["mesh"] = "./Media/Models/terrain2.ms3d";
        Canis::IEntity* ent2 = Canis::EntityManager::getSingleton().getEntityFactory("Static Mesh")->createEntity("Terrain", glm::mat4(1.0f), args);

        Canis::Scene* defaultScene = new Canis::Scene("untitled", glm::mat4(1.0f));

        Canis::SceneNode* root = new Canis::SceneNode("Root");
        Canis::SceneNode* ball = new Canis::SceneNode("BallNode", glm::translate(glm::vec3(0.0f, 1000.0f, 0.0f)));
        defaultScene->addSceneNode(root);
        defaultScene->addSceneNode(ball);

        root->attachEntity(ent2);
        ball->attachEntity(ent);
        ball->attachSceneNode(new Canis::SceneNode("TestNode", glm::translate(glm::vec3(500.0f, 0.0f, 0.0f))));

        Canis::Light* light = new Canis::Light("Light0", glm::vec3(1.0f, 1.0f, 1.0f), 10000.0f, glm::translate(glm::vec3(0.0f, 2000.0f, 0.0f)));
        //Canis::RenderManager::getSingleton().addLight(light);
        root->attachLight(light);
        //---End Scene---//
        
        ui.viewport->setActiveScene(defaultScene);

        //ui.viewport->initialize(defaultScene);

        _rootItem = nullptr;
        updateSceneGraphTree();

        //Canis::PluginManager::getSingleton().loadPlugin("./Plugins/CanisPluginTest.dll"); //TODO: load all plugins by searching directory
        //loadPlugins();

        std::map<std::string, Canis::IEntityFactory*>::iterator it;
        std::map<std::string, Canis::IEntityFactory*> entityFactories = Canis::EntityManager::getSingleton().getEntityFactories();
        for(it = entityFactories.begin(); it != entityFactories.end(); it++)
            ui.entityList->addItem(QString(((*it).first).c_str()));
            
        std::cout << "mainwindow initialized" << std::endl;
            
        ui.viewport->initialize();
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
                    EntityDialogManager::getSingleton().addEntityDialogFactory(entityDialogFactory->getEntityName().toStdString(), entityDialogFactory);
                }
            }
        }
    }

    QTreeWidgetItem* MainWindow::getSceneGraphRootItem(){
        return _rootItem;
    }

    void MainWindow::traverseNode(Canis::SceneNode* node, QTreeWidgetItem* root){
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

            traverseNode(nextNode, nextRoot);
        }
    }

    /*void MainWindow::menuTriggered(QAction* act){
        if(act->text().compare("Exit") == 0){
            QApplication::instance()->quit();
        }
        if(act->text().compare("About") == 0){
            aboutWindow.show();

            std::stringstream ss;
            ss << "Loaded engine plugins:" << std::endl;
            for(size_t i=0; i<Canis::PluginManager::getSingleton().getPlugins().size(); i++){
                Canis::IPlugin* plugin = Canis::PluginManager::getSingleton().getPlugins()[i];
                ss << plugin->getName() << " " << plugin->getVersion() << std::endl;
            }
            aboutWindow.update(QString(ss.str().c_str()));
        }
    }*/
    
    /*
     * Slots
     */

    void MainWindow::sceneGraphSelectionChanged(){
        if(!ui.sceneGraphView->selectedItems().empty()){
            QTreeWidgetItem* sel = ui.sceneGraphView->selectedItems()[0];

            if(sel->text(0).contains("<Node>")){
                ui.addEntityButton->setEnabled(true);
                ui.addLightButton->setEnabled(true);
                ui.addCameraButton->setEnabled(true);
                ui.setPoseButton->setEnabled(true);

                QString name = sel->text(0).remove(" <Node>");
                Q_EMIT objectSelected(name, "node");
            }
            else if(sel->text(0).contains("<Entity>")){
                ui.addEntityButton->setEnabled(false);
                ui.addLightButton->setEnabled(false);
                ui.addCameraButton->setEnabled(false);
                ui.setPoseButton->setEnabled(false);

                QString name = sel->text(0).remove(" <Entity>");
                Q_EMIT objectSelected(name, "entity");
            }
            else{
                ui.addEntityButton->setEnabled(false);
                ui.addLightButton->setEnabled(false);
                ui.addCameraButton->setEnabled(false);
                ui.setPoseButton->setEnabled(false);

                Q_EMIT objectSelected("", "");
                //ui.viewport->setSelectedObject("", ""); //TODO: should be a signal
            }
        }
    }

    void MainWindow::addNodeButtonClicked(){
        if(addNodeDialog.exec() == QDialog::Accepted){
            Q_EMIT sceneNodeAdded(addNodeDialog.getNodeName());
        }
    }

    void MainWindow::addEntityButtonClicked(){
        QListWidgetItem* entity = ui.entityList->selectedItems()[0];

        AddEntityDialog addEntityDialog;
        addEntityDialog.setDialog(entity->text().toStdString());
        if(addEntityDialog.exec() == QDialog::Accepted){
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
        Canis::Engine::getSingleton().reset();
        Canis::Engine::getSingleton().setDynamicsEnabled(false);
        ui.pauseSimulationButton->setText("Play");
    }
    
    void MainWindow::updateSceneGraphTree(){
        std::cout << "updateSceneGraphTree()" << std::endl;
        
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

            traverseNode(currentNode, node);
        }

        ui.sceneGraphView->expandAll();
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