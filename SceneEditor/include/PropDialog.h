#ifndef __PROPDIALOG_H
#define __PROPDIALOG_H

#include "IEntityDialog.h"

#include <QtWidgets/QtWidgets>

namespace SceneEditor
{

    class PropDialog : public IEntityDialog{
        Q_OBJECT

    private:
        QPushButton* _meshBrowse;
        QLineEdit* _nameInput;
        QLineEdit* _meshInput;
        QLineEdit* _massInput;
        QComboBox* _shapeInput;

    public:
        PropDialog(QWidget* parent = (QWidget*)0);
        ~PropDialog();

        std::map<std::string, std::string> getEntityParams();

    private Q_SLOTS:
        void meshBrowseClicked();
    };

}

#endif
