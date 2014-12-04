#include "AddLightDialog.h"

#include <Canis.h>

namespace SceneEditor
{

    AddLightDialog::AddLightDialog(QWidget* parent)
        : QDialog(parent)
    {
        ui.setupUi(this);
        
        connect(ui.colorWheel, SIGNAL(colorChanged(QColor)), this, SLOT(updateColor(QColor)));
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
    
}
