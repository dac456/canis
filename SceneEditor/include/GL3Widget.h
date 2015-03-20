#ifndef __GL3WIDGET_H
#define __GL3WIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QWindow>
#include <Canis.h>

#ifdef CANIS_PLATFORM_GNU
//#include <QtX11Extras/QX11Info>
#include <GL/glxew.h>
#endif

namespace SceneEditor
{

    struct Context{
        #ifdef CANIS_PLATFORM_WIN32
            HDC dc;
            HGLRC rc;
            PIXELFORMATDESCRIPTOR pfd;
        #endif
    };

    class GL3Widget : public QWidget{
        Q_OBJECT

    private:
        #ifdef CANIS_PLATFORM_WIN32
            HDC _dc;
            HGLRC _rc;
            PIXELFORMATDESCRIPTOR _pfd;
        #elif defined(CANIS_PLATFORM_GNU)
            typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
            typedef Bool (* PFNGLXMAKECURRENT) (Display*, GLXDrawable, GLXContext);
            PFNGLXMAKECURRENT glXMakeCurrent;
            GLXFBConfig _fbc;
            GLXContext _glxc;
            Window _xwindow;
            Display* _display;
        #endif

        QString _supportedExtensions;

    public:
        GL3Widget(QWidget* parent = 0);

        Context getContext();
        QString getSupportedExtensions();

        virtual void render() = 0;
        virtual void resize(int w, int h);
        void makeCurrent();
        void swapBuffers();

        virtual void paintEvent(QPaintEvent* e);
        virtual void resizeEvent(QResizeEvent* e);

        virtual QPaintEngine* paintEngine() const;

    public:
        void createContext(bool glewInitialized);
        QWindow* windowFromWidget(const QWidget* widget);
    };

}

#endif
