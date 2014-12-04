#include "StaticMeshDialogFactory.h"
#include "StaticMeshDialog.h"

#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qfiledialog.h>

namespace SceneEditor
{

    StaticMeshDialogFactory::StaticMeshDialogFactory(){

    }

    IEntityDialog* StaticMeshDialogFactory::createEntityDialog(){
        return new StaticMeshDialog();
    }

    QString StaticMeshDialogFactory::getEntityName(){
        return "Static Mesh";
    }



}
