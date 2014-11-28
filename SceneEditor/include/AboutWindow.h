#ifndef __ABOUTWINDOW_H
#define __ABOUTWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_AboutWindow.h"

#include <Canis.h>

#define WE_MAJOR_REVISION 0
#define WE_MINOR_REVISION 1
#define WE_BUILD_REVISION 21

namespace SceneEditor
{

    class AboutWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        AboutWindow(QWidget* parent = 0);
        ~AboutWindow();

        void update(QString supportedExtensions);

    public Q_SLOTS:
        void okButtonClicked();

    private:
        Ui::AboutWindow ui;
    };

}

#endif
