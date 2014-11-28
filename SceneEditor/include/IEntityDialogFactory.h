#ifndef __IENTITYDIALOGFACTORY_H
#define __IENTITYDIALOGFACTORY_H

#include "IEntityDialog.h"

namespace SceneEditor
{

    class IEntityDialogFactory{
    public:
        virtual ~IEntityDialogFactory(){}

        virtual IEntityDialog* createEntityDialog() = 0;
        virtual QString getEntityName() = 0;
    };

}

#define ENTITYDIALOG_IID "SceneEditor.EntityDialog/1.0"
Q_DECLARE_INTERFACE(SceneEditor::IEntityDialogFactory, ENTITYDIALOG_IID);

#endif
