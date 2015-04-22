#ifndef __FILEPROPERTY_H
#define __FILEPROPERTY_H

#include "IProperty.h"
#include <QtWidgets/QLineEdit>

namespace SceneEditor
{

    class FileProperty : public IProperty{
        Q_OBJECT
        
    private:
        QLineEdit* _lineEdit;

    public:
        FileProperty(QString name, QString value, PropCB callback);
        ~FileProperty();
        
        QWidget* getWidget(QString field);
        
    Q_SIGNALS:
        void fileChangedNotify(QVariant data);
        
    private Q_SLOTS:
        void browseClicked();
        void fileChanged(const QString& text);
    };

}

#endif
