#include "FileProperty.h"
#include <QtWidgets/QGridLayout>
#include <QFileDialog>
#include <QtWidgets/QPushButton>

namespace SceneEditor
{
    
    FileProperty::FileProperty(QString name, QString value, PropCB callback)
        : IProperty(name, "File", false, callback){
        _fields["value"] = value;
        //_lineEdit = new QLineEdit();
        
        connect(this, SIGNAL(fileChangedNotify(QVariant)), this, SLOT(_modified(QVariant)));
    }
    
    FileProperty::~FileProperty(){
    }
    
    QWidget* FileProperty::getWidget(QString field){
        QWidget* ret = new QWidget();
        QGridLayout* grid = new QGridLayout();
        
        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setText(_fields["value"].toString());
        lineEdit->setEnabled(!_readOnly);
        
        QPushButton* browse = new QPushButton("...");
        
        connect(browse, SIGNAL(clicked()), this, SLOT(browseClicked())); 
        
        grid->addWidget(lineEdit, 0, 0);
        grid->addWidget(browse, 0, 1);
        
        ret->setLayout(grid);
        return ret;
    }
    
    /*
     * SLOTS
     */
     
    void FileProperty::browseClicked(){
        QFileDialog* openDialog = new QFileDialog(0, "Open Script", "./Media/Scripts");
        openDialog->setOption(QFileDialog::DontUseNativeDialog, true);
        
        connect(openDialog, SIGNAL(fileSelected(const QString&)), this, SLOT(fileChanged(const QString&)));
        
        openDialog->exec();
        
        delete openDialog;
    }

    void FileProperty::fileChanged(const QString& path){
        //_lineEdit->setText(path);
        Q_EMIT fileChangedNotify(QVariant(path));
    }

}
