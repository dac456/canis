#ifndef __ADDLIGHTDIALOG_H
#define __ADDLIGHTDIALOG_H

#include "ui_AddLightDialog.h"

namespace SceneEditor
{

    class AddLightDialog : public QDialog{
        Q_OBJECT

    private:
        QString _name;
        float _radius;
        QColor _color;

    public:
        AddLightDialog(QWidget* parent = 0);
        ~AddLightDialog();
        
        QString getName();
        float getRadius();
        QColor getColor();
        
    private Q_SLOTS:
        void updateColor(QColor color);

    private:
        Ui::AddLightDialog ui;
    };

}

#endif
