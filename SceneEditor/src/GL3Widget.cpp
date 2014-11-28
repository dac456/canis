#include <QtGui/QResizeEvent>
#include "GL3Widget.h"
//#include <QtGui\5.0.1\QtGui\qpa\qplatformnativeinterface.h>

namespace SceneEditor
{

    GL3Widget::GL3Widget(QWidget* parent) : QWidget(parent){
        setAutoFillBackground(false);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);
        setAttribute(Qt::WA_NativeWindow);
        setAttribute(Qt::WA_PaintOnScreen, true);
        setAttribute(Qt::WA_StyledBackground, false);
        setAttribute(Qt::WA_PaintUnclipped);
        setAttribute(Qt::WA_TranslucentBackground);

        #ifdef CANIS_PLATFORM_GNU
            std::cout << "Begin creating X11 Window" << std::endl;

            glXCreateContextAttribsARB = (GLXContext(*)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list))glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
            glXChooseFBConfig = (GLXFBConfig*(*)(Display *dpy, int screen, const int *attrib_list, int *nelements))glXGetProcAddressARB((GLubyte*)"glXChooseFBConfig");
            glXGetVisualFromFBConfig = (XVisualInfo*(*)(Display *dpy, GLXFBConfig config))glXGetProcAddressARB((GLubyte*)"glXGetVisualFromFBConfig");
            //typedef GLXContext (* PFNGLXCREATECONTEXT) (Display *, XVisualInfo *, GLXContext, Bool);
            //glXCreateContext =  (GLXContext(*)(Display *, XVisualInfo *, GLXContext, Bool))glXGetProcAddress((GLubyte*)"glXCreateContext");
            glXMakeCurrent = reinterpret_cast<PFNGLXMAKECURRENT>(glXGetProcAddressARB((GLubyte*)"glXMakeCurrent"));

            int visualAttribs[] =
            {
                GLX_X_RENDERABLE    , True,
                GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                GLX_DOUBLEBUFFER    , True,
                GLX_RED_SIZE        , 8,
                GLX_GREEN_SIZE      , 8,
                GLX_BLUE_SIZE       , 8,
                GLX_ALPHA_SIZE      , 8,
                GLX_DEPTH_SIZE      , 24,
                GLX_STENCIL_SIZE    , 8,
                //GLX_SAMPLE_BUFFERS  , True,
               // GLX_SAMPLES         , 2,
                None
            };
            //const QX11Info& xinfo = QX11Info();
            //Display *display = QX11Info::display();
            _display = XOpenDisplay(NULL);
            //XVisualInfo* vi = NULL;
            _xwindow = 0;
            int fbcount;

            std::cout << "got display" << std::endl;

            GLXFBConfig* fbc = glXChooseFBConfig(_display, DefaultScreen(_display), visualAttribs, &fbcount);
            std::cout << "Got fbc" << std::endl;
            if(fbc == NULL){
                std::cout << "No valid GLX configuration." << std::endl;
            }
            _fbc = fbc[0];
            XFree(fbc);

            /*std::cout << "Got fbc" << std::endl;

            vi = glXGetVisualFromFBConfig(_display, _fbc);
            std::cout << "got visual" << std::endl;

            XSetWindowAttributes swa;
            swa.colormap = XCreateColormap(_display, RootWindow(_display, vi->screen), vi->visual, AllocNone);
            swa.background_pixmap = None;
            swa.border_pixel = 0;
            swa.event_mask = ExposureMask | VisibilityChangeMask |
                            KeyPressMask | PointerMotionMask    |
                            StructureNotifyMask | ResizeRedirectMask ;

            std::cout << "got colormap" << std::endl;

            _xwindow = XCreateWindow(_display, RootWindow(_display, vi->screen), 0, 0, this->width(), this->height(), 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
            */

            _xwindow = windowFromWidget(this)->winId();
            if(_xwindow == 0)
            {
                qFatal("Unable to create window.");
            }

            std::cout << "called XCreateWindow" << std::endl;

            // tell Qt we want to use this window instead...
            //create(_xwindow, true, true); // [url]http://doc.trolltech.com/4.5/qwidget.html#create[/url]

            std::cout << "called create() " << std::endl;
        #endif
        
        createContext(false);
    }

    void GL3Widget::createContext(bool glewInitialized){
        #ifdef CANIS_PLATFORM_WIN32
            HWND handle;
            handle = (HWND)effectiveWinId();
            _dc = GetDC(handle);

             // Create a new PIXELFORMATDESCRIPTOR (PFD)
            memset(&_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
            _pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
            _pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
            _pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
            _pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
            _pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
            _pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

            int nPixelFormat = ChoosePixelFormat(_dc, &_pfd); // Check if our PFD is valid and get a pixel format back
            bool bResult = SetPixelFormat(_dc, nPixelFormat, &_pfd); // Try and set the pixel format based on our PFD

            HGLRC tempContext = wglCreateContext(_dc); // Create an OpenGL 2.1 context for our device context
            wglMakeCurrent(_dc, tempContext); // Make the OpenGL 2.1 context current and active

            if(!glewInitialized){
                glewExperimental = GL_TRUE;
                glewInit();
            }

            int attributes[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // Set the MAJOR version of OpenGL to 3
                WGL_CONTEXT_MINOR_VERSION_ARB, 3, // Set the MINOR version of OpenGL to 3
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
                0
            };

            if (wglewIsSupported("WGL_ARB_create_context") == 1) { // If the OpenGL 3.x context creation extension is available
                _rc = wglCreateContextAttribsARB(_dc, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
                wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
                wglDeleteContext(tempContext); // Delete the temporary OpenGL 2.1 context
                wglMakeCurrent(_dc, _rc); // Make our OpenGL 3.0 context current
                _supportedExtensions = QString((const char*)glGetString(GL_EXTENSIONS));
            }
            else {
                _rc = tempContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
                wglMakeCurrent(_dc, _rc);
            }
            
            QT_EMIT contextCreated();
        #elif defined(CANIS_PLATFORM_GNU)
            std::cout << "Begin create context GNU" << std::endl;

            XVisualInfo* vi = glXGetVisualFromFBConfig(_display, _fbc);
            std::cout << "got visual" << std::endl;

            /*typedef GLXContext (* PFNGLXCREATECONTEXT) (Display *, XVisualInfo *, GLXContext, Bool);
            PFNGLXCREATECONTEXT glXCreateContext = reinterpret_cast<PFNGLXCREATECONTEXT>(glXGetProcAddressARB((GLubyte*)"glXCreateContext"));

            GLXContext tmp = glXCreateContext(_display, vi, 0, true);
            Q_ASSERT(tmp != NULL);
            glXMakeCurrent(_display, _xwindow, tmp);

            std::cout << "created old context" << std::endl;*/

            glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
            glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
            Q_ASSERT(glXCreateContextAttribsARB != NULL);

            glXMakeCurrent(_display, 0, 0);
            //glXDestroyContext(_display, tmp);

            int attribs[] = {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 3,
                GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                0
            };

            _glxc = glXCreateContextAttribsARB(_display, _fbc, 0, true, attribs);
            if(_glxc == NULL){
                qFatal("Unable to create gl context.");
            }
            //if(glXIsDirect(_display, _glxc) != True){
            //    qFatal("The gl context is not direct.");
            //}

            makeCurrent();
            std::cout << "makecurrent" << std::endl;
            if(!glewInitialized){
                glewExperimental = GL_TRUE;
                glewInit();
            }

            std::cout << "initialized glew" << std::endl;

            std::cout << "created gl3 context" << std::endl;
        #endif
    }

    Context GL3Widget::getContext(){
        Context c;

        #ifdef CANIS_PLATFORM_WIN32
            c.dc = _dc;
            c.rc = _rc;
            c.pfd = _pfd;
        #endif

        return c;
    }

    QString GL3Widget::getSupportedExtensions(){
        return _supportedExtensions;
    }

    void GL3Widget::resize(int w, int h){
        glViewport(0, 0, w, h);
        //XMoveResizeWindow(_display, _xwindow, 0, 0, w, h);
    }

    void GL3Widget::makeCurrent(){
        #ifdef CANIS_PLATFORM_WIN32
            wglMakeCurrent(_dc, _rc);
        #elif defined(CANIS_PLATFORM_GNU)
            glXMakeCurrent(_display, _xwindow, _glxc);
        #endif // defined
    }

    void GL3Widget::swapBuffers(){
        #ifdef CANIS_PLATFORM_WIN32
            wglMakeCurrent(_dc, _rc);
            SwapBuffers(_dc);
        #elif defined(CANIS_PLATFORM_GNU)
            glXMakeCurrent(_display, _xwindow, _glxc);
            glXSwapBuffers(_display, _xwindow);
        #endif // defined
    }

    void GL3Widget::paintEvent(QPaintEvent* e){
        render();
    }

    void  GL3Widget::resizeEvent(QResizeEvent* e){
        resize(e->size().width(), e->size().height());
    }

    QPaintEngine* GL3Widget::paintEngine() const{
        return nullptr;
    }

    QWindow* GL3Widget::windowFromWidget(const QWidget* widget)
    {
        QWindow* window = widget->windowHandle();
        if (window)
            return window;
        const QWidget* nativeParent = widget->nativeParentWidget();
        if (nativeParent)
            return nativeParent->windowHandle();
        return 0;
    }

}
