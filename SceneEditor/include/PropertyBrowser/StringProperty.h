#ifndef __STRINGPROPERTY_H
#define __STRINGPROPERTY_H

#include "IProperty.h"

namespace SceneEditor
{
    
    class StringProperty : public IProperty{
        Q_OBJECT
        
    public:
        StringProperty(QString name, QString value, std::function<void(QVariant)> callback);
        ~StringProperty();
        
        QWidget* getWidget(QString field);
    
    Q_SIGNALS:
        void stringChangedNotify(QVariant data);
        
    private Q_SLOTS:
        void stringChanged(const QString& text);
    };
    
}

#endif
