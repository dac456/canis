#ifndef __PROPDIALOG_H
#define __PROPDIALOG_H

#include "IEntityDialog.h"

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>

namespace SceneEditor
{

    class PropDialog : public IEntityDialog{
        Q_OBJECT

    private:
        QPushButton* _meshBrowse;
        QLineEdit* _nameInput;
        QLineEdit* _meshInput;

    public:
        PropDialog(QWidget* parent = (QWidget*)0);
        ~PropDialog();

        std::map<std::string, std::string> getEntityParams();

    private:
        void meshBrowseClicked();
    };

}

#endif
