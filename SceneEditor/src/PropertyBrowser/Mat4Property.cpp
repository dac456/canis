#include "Mat4Property.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>

namespace SceneEditor
{
    
    Mat4Property::Mat4Property(QString name, QMatrix4x4 value, PropCB callback)
        : IProperty(name, "Mat4", false, callback){
        _fields["value"] = value;
    }
    
    Mat4Property::~Mat4Property(){
    }
    
    QWidget* Mat4Property::getWidget(QString field){
        QMatrix4x4 currentValue = _fields["value"].value<QMatrix4x4>();
        
        QWidget* ret = new QWidget();
        QGridLayout* grid = new QGridLayout();
        
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                QLineEdit* m = new QLineEdit();
                QString ms;
                ms.setNum(currentValue(i, j));
                m->setText(ms);
                grid->addWidget(m, i, j);
            }
        }
        
        ret->setLayout(grid);
        return ret;
    }

}
