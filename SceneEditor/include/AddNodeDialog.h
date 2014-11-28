#ifndef __ADDNODEDIALOG_H
#define __ADDNODEDIALOG_H

#include <QtWidgets/QMainWindow>
#include "ui_AddNodeDialog.h"

namespace SceneEditor
{

    class AddNodeDialog : public QDialog{
        Q_OBJECT

    public:
        AddNodeDialog(QWidget* parent = 0);
        ~AddNodeDialog();

        QString getNodeName();

    private:
        Ui::AddNodeDialog ui;
    };

}

#endif
