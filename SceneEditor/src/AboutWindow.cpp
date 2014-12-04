#include "AboutWindow.h"

namespace SceneEditor
{

    AboutWindow::AboutWindow(QWidget* parent){
        ui.setupUi(this);

        connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    }

    AboutWindow::~AboutWindow(){
    }

    void AboutWindow::update(QString supportedExtensions){
        std::stringstream glVersion;
        glVersion << glGetString(GL_VERSION);
        ui.glVersionLabel->setText(QString(glVersion.str().c_str()));

        std::stringstream canisVersion;
        canisVersion << CS_MAJOR_REVISION << "." << CS_MINOR_REVISION << "." << CS_BUILD_REVISION << " " << CS_CODENAME;
        ui.canisVersionLabel->setText(QString(canisVersion.str().c_str()));

        std::stringstream editorVersion;
        editorVersion << CS_MAJOR_REVISION << "." << CS_MINOR_REVISION << "." << CS_BUILD_REVISION;
        ui.editorVersionLabel->setText(QString(editorVersion.str().c_str()));

        ui.infoText->setText(supportedExtensions);
    }

    void AboutWindow::okButtonClicked(){
        this->close();
    }
}
