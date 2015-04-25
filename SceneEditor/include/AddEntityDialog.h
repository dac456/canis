#ifndef __ADDENTITYDIALOG_H
#define __ADDENTITYDIALOG_H

#include "ui_AddEntityDialog.h"
#include "EntityDialogManager.h"

namespace SceneEditor
{

    class AddEntityDialog : public QDialog{
        Q_OBJECT

    private:
        QLineEdit* nameInput;
        QPushButton* okButton;
        QPushButton* cancelButton;
        IEntityDialog* dialogWidget;

    public:
        AddEntityDialog(QWidget* parent = 0);
        ~AddEntityDialog();

        void setDialog(std::string entity);
        std::map<std::string, std::string> getEntityParams();
        
    public Q_SLOTS:
        void nameChanged(const QString& name);

    private:
        Ui::AddEntityDialog ui;
    };

}

#endif
