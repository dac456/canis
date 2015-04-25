#include "AddNodeDialog.h"
//#include "MainWindow.h"
#include <Canis.h>

namespace SceneEditor
{

    AddNodeDialog::AddNodeDialog(QWidget* parent)
        : QDialog(parent) 
    {
        ui.setupUi(this);
        
        connect(ui.nodeName, SIGNAL(textChanged(const QString&)), this, SLOT(nameChanged(const QString&)));
    }

    AddNodeDialog::~AddNodeDialog(){
    }

    QString AddNodeDialog::getNodeName(){
        return ui.nodeName->text();
    }
    
    /*
     * SLOTS
     */
    
    void AddNodeDialog::nameChanged(const QString& name){
        Canis::ScenePtr sc = Canis::Engine::getSingleton().getRenderer()->getActiveScene();
        if(name == "" || sc->nodeExists(name.toStdString())){
            ui.okButton->setEnabled(false);
        }
        else{
            ui.okButton->setEnabled(true);
        }
    }

}
