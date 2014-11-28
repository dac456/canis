#ifndef __IENTITYDIALOG_H
#define __IENTITYDIALOG_H

#include <string>
#include <map>

#include <QtWidgets/QWidget>

namespace SceneEditor
{

    class IEntityDialog : public QWidget{
    public:
        IEntityDialog(QWidget* parent = (QWidget*)0)
            : QWidget(parent){}
        virtual ~IEntityDialog(){}

        virtual std::map<std::string, std::string> getEntityParams() = 0;
    };

}

#endif
