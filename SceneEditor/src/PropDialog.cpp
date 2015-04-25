#include "PropDialog.h"

#include <sstream>
#include <QDoubleValidator>

namespace SceneEditor
{

    PropDialog::PropDialog(QWidget* parent)
        : IEntityDialog(parent){
        QVBoxLayout* layout = new QVBoxLayout();

        QHBoxLayout* field2 = new QHBoxLayout();
        QLabel* meshLabel = new QLabel("Mesh: ");
        _meshInput = new QLineEdit();
        _meshInput->setReadOnly(false);
        _meshBrowse = new QPushButton("...");
        field2->addWidget(meshLabel);
        field2->addWidget(_meshInput);
        field2->addWidget(_meshBrowse);
        layout->addLayout(field2);
        
        QHBoxLayout* field3 = new QHBoxLayout();
        QLabel* massLabel = new QLabel("Mass: ");
        _massInput = new QLineEdit();
        _massInput->setValidator(new QDoubleValidator(0, 1000000, 6));
        field3->addWidget(massLabel);
        field3->addWidget(_massInput);
        layout->addLayout(field3);
        
        QHBoxLayout* field4 = new QHBoxLayout();
        QLabel* shapeLabel = new QLabel("Shape: ");
        _shapeInput = new QComboBox();
        _shapeInput->addItem("Box");
        _shapeInput->addItem("Sphere");
        _shapeInput->addItem("Capsule");
        field4->addWidget(shapeLabel);
        field4->addWidget(_shapeInput);
        layout->addLayout(field4);

        setLayout(layout);

        connect(_meshBrowse, SIGNAL(clicked()), this, SLOT(meshBrowseClicked()));
    }

    PropDialog::~PropDialog(){
    }

    std::map<std::string, std::string> PropDialog::getEntityParams(){
        std::map<std::string, std::string> params;
        params["mesh"] = _meshInput->text().toStdString();
        params["mass"] = _massInput->text().toStdString();
        
        std::stringstream shapeStr;
        shapeStr << _shapeInput->currentIndex();
        params["shape"] = shapeStr.str();

        return params;
    }

    void PropDialog::meshBrowseClicked(){
        QFileDialog meshDialog(this, "Select Mesh", "./Media/Models");
        meshDialog.setOption(QFileDialog::DontUseNativeDialog, true);
        if(meshDialog.exec()){
            QStringList files;
            files = meshDialog.selectedFiles();
            _meshInput->setText(files.at(0));
        }
    }

}
