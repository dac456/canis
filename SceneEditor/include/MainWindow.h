#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QElapsedTimer>
#include "ui_MainWindow.h"
#include "AboutWindow.h"
#include "AddNodeDialog.h"
#include "AddEntityDialog.h"
#include "AddLightDialog.h"

#include "EntityDialogManager.h"

#include <Canis.h>

namespace SceneEditor
{
    
    typedef struct{
        qint64 lastTime;
        size_t frameCount;
    } FrameRate;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    private:
        Canis::Engine* _engine;
        EntityDialogManager* _entityDialogManager;

        QTreeWidgetItem* _rootItem;
        
        QElapsedTimer _fpsTimer;
        QLabel* _fpsLabel;
        
        FrameRate _fpsCounter;

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void loadPlugins();

    private:
        //Helpers
        void _traverseNode(Canis::SceneNode* node, QTreeWidgetItem* root);

    public Q_SLOTS:
        void initialize();

        void viewportChanged(int);
        void sceneGraphSelectionChanged();
        void setPropertySheetNode(Canis::SceneNode* node);
        void setPropertySheetLight(Canis::Light* light);
        void setPropertySheetEntity(Canis::IEntity* entity);
        void addNodeButtonClicked();
        void addLightButtonClicked();
        void addEntityButtonClicked();
        void pauseSimulationButtonClicked();
        void resetSimulationButtonClicked();
        
        void xLockButtonClicked();
        void yLockButtonClicked();
        void zLockButtonClicked();
        
        void updateSceneGraphTree();
        void updateFpsCounter();
        
        void newScene();
        void save();
        void open();
        void about();
        void exit();
        
    Q_SIGNALS:
        void objectSelected(QString name, QString type);
        void sceneNodeAdded(QString name);
        void entityAdded(QString type, Canis::StringMap args);
        void lightAdded(QString name, float radius, QColor diffuse);
        void initialized();
        void sceneLoaded(Canis::Scene* scene);
        
        void enableXAxis(int enable); //MOC complains about bool
        void enableYAxis(int enable);
        void enableZAxis(int enable);

    private:
        Ui::MainWindowClass ui;

        AboutWindow aboutWindow;
        AddNodeDialog addNodeDialog;
        AddLightDialog addLightDialog;

        friend class AddNodeDialog;
    };

}

#endif // MAINWINDOW_H
