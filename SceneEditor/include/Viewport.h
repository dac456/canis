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
    
    typedef enum{
        PERSP,
        TOP,
        FRONT,
        SIDE
    } VIEW_TYPE;

    class Viewport : public GL3Widget{
        Q_OBJECT

    private:
        QTimer* _timer;
        bool _initialized;

        //Canis
        Canis::Renderer* _renderer; //TODO: start offloading to renderer class as much as possible
        Canis::Camera* _cam;
        //Canis::Mesh* _nodeMarker;
        //Canis::Mesh* _selectionBox;
        Canis::RenderableList _nodeMarker;
        Canis::RenderableList _selectionBox;

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
        bool _xAxisEnabled;
        bool _yAxisEnabled;
        bool _zAxisEnabled;

    public:
        Viewport(QWidget* parent = 0);
        ~Viewport();
        
        void connectToMainWindow(MainWindow* main);
        
        Canis::Renderer* getRenderer();

        virtual void render();
        virtual void resize(int w, int h);     

        virtual void mouseMoveEvent(QMouseEvent* e);
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void wheelEvent(QWheelEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);
        virtual void keyReleaseEvent(QKeyEvent* e);

    private:
        //Helpers
        void _setView(int type);
        void _setOrtho();
        void _rotateFirstPerson(int x, int y);

        void _renderNodeMarker(Canis::SceneNodePtr node);
        
        void _removeSceneNode(QString name);
        void _removeEntity(QString name);
        void _removeLight(QString name);

    public Q_SLOTS:
        void initialize();
        void setActiveScene(Canis::ScenePtr scene);
        
        void selectObject(QString name, QString type);
        void updateView(QString item);
        void setInitialPoseButtonClicked();
        
        void setViewPerspective();
        void setViewTop();
        void setViewFront();
        void setViewSide();
        
        void setXAxisEnabled(int enabled); //MOC complains about bool here, for some reason
        void setYAxisEnabled(int enabled);
        void setZAxisEnabled(int enabled);
        
        void addSceneNode(QString name);
        void addEntity(QString type, Canis::StringMap args);
        void addLight(QString name, float radius, QColor diffuse);
        
    Q_SIGNALS:
        void contextCreated();
        void sceneChanged();
        void viewportChanged(int type);
        void renderOnce();
        void setPropertySheetNode(Canis::SceneNodePtr node);
        void setPropertySheetLight(Canis::LightPtr light);
        void setPropertySheetEntity(Canis::IEntityPtr entity);
    };

}

#endif
