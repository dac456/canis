#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "AboutWindow.h"
#include "AddNodeDialog.h"
#include "AddEntityDialog.h"

#include "EntityDialogManager.h"

#include <Canis.h>

namespace SceneEditor
{

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    private:
        Canis::Engine* _engine;
        EntityDialogManager* _entityDialogManager;

        QTreeWidgetItem* _rootItem;

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void loadPlugins();

        QTreeWidgetItem* getSceneGraphRootItem();
    
        //Actions
        //void about();

    private:
        void traverseNode(Canis::SceneNode* node, QTreeWidgetItem* root);

    public Q_SLOTS:
        void initialize();
        //void menuTriggered(QAction* act);
        void sceneGraphSelectionChanged();
        void addNodeButtonClicked();
        void addEntityButtonClicked();
        void pauseSimulationButtonClicked();
        void resetSimulationButtonClicked();
        
        void updateSceneGraphTree();
        
        void about();
        void exit();
        
    Q_SIGNALS:
        void objectSelected(QString name, QString type);
        void sceneNodeAdded(QString name);

    private:
        Ui::MainWindowClass ui;

        AboutWindow aboutWindow;
        AddNodeDialog addNodeDialog;

        friend class AddNodeDialog;
    };

}

#endif // MAINWINDOW_H
