#ifndef __STATICMESHDIALOG_H
#define __STATICMESHDIALOG_H

#include "IEntityDialog.h"

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>

namespace SceneEditor
{

    class StaticMeshDialog : public IEntityDialog{
        Q_OBJECT

    private:
        QPushButton* _meshBrowse;
        QLineEdit* _nameInput;
        QLineEdit* _meshInput;

    public:
        StaticMeshDialog(QWidget* parent = (QWidget*)0);
        ~StaticMeshDialog();

        std::map<std::string, std::string> getEntityParams();

    private Q_SLOTS:
        void meshBrowseClicked();
    };

}

#endif
