#include "AddNodeDialog.h"
#include "MainWindow.h"
#include <Canis.h>

namespace SceneEditor
{

    AddNodeDialog::AddNodeDialog(QWidget* parent)
        : QDialog(parent) 
    {
        ui.setupUi(this);
    }

    AddNodeDialog::~AddNodeDialog(){
    }

    QString AddNodeDialog::getNodeName(){
        return ui.nodeName->text();
    }

}
