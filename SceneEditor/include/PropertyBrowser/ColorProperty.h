#ifndef __COLORPROPERTY_H
#define __COLORPROPERTY_H

#include "IProperty.h"
#include "color_wheel.h"

namespace SceneEditor
{

    class ColorProperty : public IProperty{
        Q_OBJECT
        
    public:
        ColorProperty(QString name, QColor value, PropCB callback);
        ~ColorProperty();
        
        QWidget* getWidget(QString field);
        
    Q_SIGNALS:
        void colorChangedNotify(QVariant data);
        
    private Q_SLOTS:
        void colorChanged(QColor color);
    };

}

#endif
