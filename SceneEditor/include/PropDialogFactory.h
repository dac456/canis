#ifndef __PROPDIALOGFACTORY_H
#define __PROPDIALOGFACTORY_H

#include "IEntityDialogFactory.h"
#include "IEntityDialog.h"
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>

namespace SceneEditor
{

    class PropDialogFactory : public QObject, public IEntityDialogFactory{
        Q_OBJECT

    private:

    public:
        PropDialogFactory();

        IEntityDialog* createEntityDialog();
        QString getEntityName();
    };

}

#endif
