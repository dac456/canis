#ifndef __STATICMESHDIALOGFACTORY_H
#define __STATICMESHDIALOGFACTORY_H

#include "IEntityDialogFactory.h"
#include "IEntityDialog.h"
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>

namespace SceneEditor
{

    class StaticMeshDialogFactory : public QObject, public IEntityDialogFactory{
        Q_OBJECT

    private:

    public:
        StaticMeshDialogFactory();

        IEntityDialog* createEntityDialog();
        QString getEntityName();
    };

}

#endif
