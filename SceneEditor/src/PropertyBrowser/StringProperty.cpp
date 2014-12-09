#include "StringProperty.h"
#include <QtWidgets/QLineEdit>

namespace SceneEditor
{
    
    StringProperty::StringProperty(QString name, QString value, PropCB callback)
        : IProperty(name, "String", callback){
            _fields["value"] = value;
            
            connect(this, SIGNAL(stringChangedNotify(QVariant)), this, SLOT(_modified(QVariant)));
    }
    
    StringProperty::~StringProperty(){
    }
    
    QWidget* StringProperty::getWidget(QString field){
        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setText(_fields["value"].toString());
        
        connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(stringChanged(const QString&)));
        
        return lineEdit;
    }
    
    /*
     * SLOTS
     */
     
    void StringProperty::stringChanged(const QString& text){
        Q_EMIT stringChangedNotify(QVariant(text));
    }
    
}
