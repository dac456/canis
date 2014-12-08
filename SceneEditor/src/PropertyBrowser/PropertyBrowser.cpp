#include "PropertyBrowser.h"

#include <QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>

namespace SceneEditor
{
    
    PropertyBrowser::PropertyBrowser(QWidget* parent)
        : QWidget(parent){
            _layout = nullptr;
    }
    
    PropertyBrowser::~PropertyBrowser(){
    }
    
    void PropertyBrowser::reset(){
        for(auto key : _properties.keys()){
            delete _properties.value(key);
        }
        _properties.clear();
        
        qDeleteAll(children());
        if(layout() != 0){
            delete layout();
        }
        
        _layout = new QVBoxLayout();
        setLayout(_layout);       
    }
    
    void PropertyBrowser::addProperty(IProperty* property){
        _properties[property->getName()] = property;
        _update();
    }
    
    void PropertyBrowser::_update(){
        _layout->setAlignment(Qt::AlignTop);
        
        for(auto key : _properties.keys()){
            QVBoxLayout* prop = new QVBoxLayout();
            
            QGroupBox* group = new QGroupBox(key);
            QVBoxLayout* groupLayout = new QVBoxLayout();
            
            for(QString f : _properties.value(key)->getFields().keys()){
                QHBoxLayout* fieldLayout = new QHBoxLayout();
                
                if(_properties.value(key)->isGroup()){
                    QLabel* l = new QLabel(f);
                    fieldLayout->addWidget(l);
                }
                
                QWidget* w = _properties.value(key)->getWidget(f);
                fieldLayout->addWidget(w);
                
                groupLayout->addLayout(fieldLayout);
            }
            
            group->setLayout(groupLayout);
            prop->addWidget(group);
            
            _layout->addLayout(prop);
        }
        
        setLayout(_layout);
    }
    
}
