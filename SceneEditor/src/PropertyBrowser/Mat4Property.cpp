#include "Mat4Property.h"
#include <QtWidgets/QGridLayout>

namespace SceneEditor
{
    
    Mat4Property::Mat4Property(QString name, QMatrix4x4 value, PropCB callback)
        : IProperty(name, "Mat4", false, callback){
        _fields["value"] = value;
        _signalMapper = new QSignalMapper(this);
        
        int id = 0;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                _widgetMap[id] = new QLineEdit();
               
                connect(_widgetMap[id], SIGNAL(textChanged(const QString&)), _signalMapper, SLOT(map()));
                _signalMapper->setMapping(_widgetMap[id], id);                
                
                id++;
            }
        }
        
        connect(this, SIGNAL(matrixChangedNotify(QVariant)), this, SLOT(_modified(QVariant)));        
    }
    
    Mat4Property::~Mat4Property(){
        if(_signalMapper != nullptr){
            delete _signalMapper;
        }
        
        for(auto w : _widgetMap.keys()){
            delete _widgetMap[w];
        }
    }
    
    QWidget* Mat4Property::getWidget(QString field){
        QMatrix4x4 currentValue = _fields["value"].value<QMatrix4x4>();
        
        QWidget* ret = new QWidget();
        QGridLayout* grid = new QGridLayout();
        
        int id = 0;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                QLineEdit* m = _widgetMap[id];
                QString ms;
                ms.setNum(currentValue(i, j));
                m->setText(ms);
                grid->addWidget(m, i, j);
            
                id++;
            }
        }
        
        connect(_signalMapper, SIGNAL(mapped(const int)), this, SLOT(matrixChanged(const int)));
        
        ret->setLayout(grid);
        return ret;
    }
    
    void Mat4Property::matrixChanged(const int id){
        QMatrix4x4 out;
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                out(i,j) = _widgetMap[id]->text().toFloat();
            }
        }
        
        Q_EMIT matrixChangedNotify(QVariant(out));
    }

}
