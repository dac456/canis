#if defined(__WIN32__) || defined(_WIN32)
#pragma comment (lib, "Canis.lib")
#pragma comment (lib, "glew32.lib")
#endif

#include "MainWindow.h"
#include <QtWidgets/QApplication>

#include <Canis.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SceneEditor::MainWindow w;
    w.show();
    w.initialize();

    return a.exec();
}
