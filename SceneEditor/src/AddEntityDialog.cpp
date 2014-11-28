#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qcombobox.h>
#include <limits>
#include "AddEntityDialog.h"
#include "EntityDialogManager.h"

#include <Canis.h>

namespace SceneEditor
{

    AddEntityDialog::AddEntityDialog(QWidget* parent)
        : QDialog(parent)
    {
        ui.setupUi(this);

        okButton = new QPushButton("OK");
        cancelButton = new QPushButton("Cancel");

        connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    }

    AddEntityDialog::~AddEntityDialog(){
    }

    void AddEntityDialog::setDialog(std::string entity){
        QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

        //TODO: check first and see if an appropriate factory exists
        dialogWidget = EntityDialogManager::getSingleton().getEntityDialogFactory(entity)->createEntityDialog();
        layout->addWidget(dialogWidget);

        QHBoxLayout* buttons = new QHBoxLayout();

        buttons->addWidget(okButton);
        buttons->addWidget(cancelButton);

        layout->addLayout(buttons);
        setLayout(layout);
    }

    std::map<std::string, std::string> AddEntityDialog::getEntityParams(){
        return dialogWidget->getEntityParams();
    }

}
