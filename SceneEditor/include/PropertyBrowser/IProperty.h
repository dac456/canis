#ifndef __IPROPERTY_H
#define __IPROPERTY_H

#include <iostream>
#include <functional>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QVariant>

namespace SceneEditor
{
    
    typedef QMap<QString, QVariant> FieldMap;

    class IProperty : public QObject{
        Q_OBJECT
        
    protected:
        QString _name;
        QString _type;
        std::function<void(QVariant)> _callback;
        FieldMap _fields;
        
        QVector<IProperty*> _children;
        
    public:
        IProperty(QString name, QString type, std::function<void(QVariant)> callback){
            _name = name;
            _type = type;
            _callback = callback;
            
            connect(this, SIGNAL(modified(QVariant)), this, SLOT(_callbackImpl(QVariant)));
        }
        virtual ~IProperty(){}
        
        virtual QWidget* getWidget(QString field) = 0;
        
        QString getName(){
            return _name;
        }
        
        QString getType(){
            return _type;
        }
        
        FieldMap getFields(){
            return _fields;
        }
        
        QVariant getField(QString name){
            return _fields[name];
        }
        
        bool isGroup(){
            return (_fields.size() > 1);
        }
        
    Q_SIGNALS:
        void modified(QVariant data);
        
    protected:
        void _addProperty(IProperty* property){
            _children.push_back(property);
        }
        
    protected Q_SLOTS:
        void _modified(QVariant data){
            Q_EMIT modified(data);
        }
        
        void _callbackImpl(QVariant data){
            _callback(data);
        }
        
    };

}

#endif
