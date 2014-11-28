#include "PropDialogFactory.h"
#include "PropDialog.h"

#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qfiledialog.h>

namespace SceneEditor
{

    PropDialogFactory::PropDialogFactory(){

    }

    IEntityDialog* PropDialogFactory::createEntityDialog(){
        return new PropDialog();
    }

    QString PropDialogFactory::getEntityName(){
        return "Prop";
    }



}
