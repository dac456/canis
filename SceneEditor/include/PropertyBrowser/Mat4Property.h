#ifndef __MAT4PROPERTY_H
#define __MAT4PROPERTY_H

#include "IProperty.h"
#include <QtGui/QMatrix4x4>

namespace SceneEditor
{
    
    class Mat4Property : public IProperty{
        Q_OBJECT
        
    public:
        Mat4Property(QString name, QMatrix4x4 value, PropCB callback);
        ~Mat4Property();
        
        QWidget* getWidget(QString field);
        
    Q_SIGNALS:
        void matrixChangedNotify(QVariant data);
        
    private Q_SLOTS:

    };
    
}

#endif
