#include "StaticMeshDialog.h"

#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qfiledialog.h>

#include <QDoubleValidator>

namespace SceneEditor
{

    StaticMeshDialog::StaticMeshDialog(QWidget* parent)
        : IEntityDialog(parent){
        QVBoxLayout* layout = new QVBoxLayout();

        QHBoxLayout* field1 = new QHBoxLayout();
        QLabel* nameLabel = new QLabel("Name: ");
        _nameInput = new QLineEdit();
        field1->addWidget(nameLabel);
        field1->addWidget(_nameInput);
        layout->addLayout(field1);

        QHBoxLayout* field2 = new QHBoxLayout();
        QLabel* meshLabel = new QLabel("Mesh: ");
        _meshInput = new QLineEdit();
        _meshInput->setReadOnly(false);
        _meshBrowse = new QPushButton("...");
        field2->addWidget(meshLabel);
        field2->addWidget(_meshInput);
        field2->addWidget(_meshBrowse);
        layout->addLayout(field2);

        setLayout(layout);

        connect(_meshBrowse, SIGNAL(clicked()), this, SLOT(meshBrowseClicked()));
    }

    StaticMeshDialog::~StaticMeshDialog(){
    }

    std::map<std::string, std::string> StaticMeshDialog::getEntityParams(){
        std::map<std::string, std::string> params;
        params["name"] = _nameInput->text().toStdString();
        params["mesh"] = _meshInput->text().toStdString();

        return params;
    }

    void StaticMeshDialog::meshBrowseClicked(){
        QFileDialog meshDialog(this, "Select Mesh", "./Media/Models");
        if(meshDialog.exec()){
            QStringList files;
            files = meshDialog.selectedFiles();
            _meshInput->setText(files.at(0));
        }
    }

}
