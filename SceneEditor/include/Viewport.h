#ifndef __PERSPECTIVEVIEW_H
#define __PERSPECTIVEVIEW_H

//#include "GL3Widget.h"
#include <GL/glew.h>
#include "GL3Widget.h"
//#include <QtOpenGL/QGLWidget>

#include <Canis.h>

namespace SceneEditor
{
    
    class MainWindow; //Forward declare

    typedef enum{
        VIEW_MODE,
        MOVE_MODE,
        SCALE_MODE,
        ROTATE_MODE
    } MOUSE_MODE;

    class Viewport : public GL3Widget{
        Q_OBJECT

    private:
        QTimer* _timer;
        bool _initialized;

        //Canis
        Canis::Scene* _activeScene;
        Canis::Camera* _cam;
        Canis::Mesh* _nodeMarker;
        Canis::Mesh* _selectionBox;

        glm::mat4 _projMatrix;
        glm::vec3 _lastPerspPos;
        glm::fquat _lastPerspOrient;

        //Mouse control
        float _lastX;
        float _lastY;
        bool _mouseLock;

        float _orthoScale;
        float _orthoOrgX;
        float _orthoOrgY;
        float _orthoMaxX;
        float _orthoMaxY;
        float _orthoOffsetX;
        float _orthoOffsetY;

        float _scrollDivisor;

        MOUSE_MODE _mode;

        //External control
        int _viewType;
        QString _selectedObjectName;
        QString _selectedObjectType;

    public:
        Viewport(QWidget* parent = 0);
        ~Viewport();
        
        void connectToMainWindow(MainWindow* main);

        void initialize();
        virtual void render();
        virtual void resize(int w, int h);     

        void setActiveScene(Canis::Scene* scene);
        Canis::Scene* getActiveScene();

        virtual void mouseMoveEvent(QMouseEvent* e);
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void wheelEvent(QWheelEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);
        virtual void keyReleaseEvent(QKeyEvent* e);

    private:
        //Helpers
        void setView(int type);
        void rotateFirstPerson(int x, int y);

        void renderNodeMarker(Canis::SceneNode* node);
        Canis::SceneNode* getNodeByName(std::string name);
        Canis::SceneNode* getChildNodeByName(Canis::SceneNode* node, std::string name);

    public Q_SLOTS:
        void selectObject(QString name, QString type);
        void updateView(QString item);
        void setInitialPoseButtonClicked();
        
        void addSceneNode(QString name);
        
    Q_SIGNALS:
        void contextCreated();
        void sceneChanged();

    };

}

#endif
