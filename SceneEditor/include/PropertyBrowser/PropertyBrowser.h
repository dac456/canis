#ifndef __PROPERTYBROWSER_H
#define __PROPERTYBROWSER_H

#include <QtWidgets/QWidget>
#include "IProperty.h"

namespace SceneEditor
{
    
    class PropertyBrowser : public QWidget{
        Q_OBJECT
        
    private:
        QMap<QString, IProperty*> _properties;
        
    public:
        PropertyBrowser(QWidget* parent = (QWidget*)0);
        ~PropertyBrowser();
        
        void addProperty(IProperty* property);
        
    private:
        void _update();
    };
    
}

#endif
