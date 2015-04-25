#include <QtWidgets/QPushButton>
#include "AddLightDialog.h"

#include <Canis.h>

namespace SceneEditor
{

    AddLightDialog::AddLightDialog(QWidget* parent)
        : QDialog(parent)
    {
        ui.setupUi(this);
        
        QPushButton* okButton = ui.buttonBox->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);
        
        connect(ui.colorWheel, SIGNAL(colorChanged(QColor)), this, SLOT(updateColor(QColor)));
        connect(ui.nameInput, SIGNAL(textChanged(const QString&)), this, SLOT(nameChanged(const QString&)));
    }

    AddLightDialog::~AddLightDialog(){
    }
    
    QString AddLightDialog::getName(){
        return ui.nameInput->text();
    }
    
    float AddLightDialog::getRadius(){
        return std::stof(ui.radiusInput->text().toStdString());
    }
    
    QColor AddLightDialog::getColor(){
        return _color;
    }
    
    /*
     * Slots
     */
     
     void AddLightDialog::updateColor(QColor color){
         _color = color;
     }
     
    void AddLightDialog::nameChanged(const QString& name){
        QPushButton* okButton = ui.buttonBox->button(QDialogButtonBox::Ok);
        
        Canis::ScenePtr sc = Canis::Engine::getSingleton().getRenderer()->getActiveScene();
        if(name == "" || sc->lightExists(name.toStdString())){
            okButton->setEnabled(false);
        }
        else{
            okButton->setEnabled(true);
        }        
    }     
    
}
