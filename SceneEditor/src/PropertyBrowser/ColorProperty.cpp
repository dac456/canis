#include "ColorProperty.h"

namespace SceneEditor
{
    
    ColorProperty::ColorProperty(QString name, QColor value, PropCB callback)
        : IProperty(name, "Color", false, callback){
            _fields["value"] = value;
            
            connect(this, SIGNAL(colorChangedNotify(QVariant)), this, SLOT(_modified(QVariant)));
    }
    
    ColorProperty::~ColorProperty(){
    }
    
    QWidget* ColorProperty::getWidget(QString field){
        Color_Wheel* wheel = new Color_Wheel();
        wheel->setColor(_fields["value"].value<QColor>());
        
        connect(wheel, SIGNAL(colorChanged(QColor)), this, SLOT(colorChanged(QColor)));
        
        return wheel;
    }
    
    /*
     * Slots
     */
     
     void ColorProperty::colorChanged(QColor color){
         Q_EMIT colorChangedNotify(QVariant(color));
     }
    
}
