#include "modeldialog.h"

namespace CP {
    namespace UI {
        
        void createDialog(QWidget* parent, QString title, QString name);

        ModelDialog::ModelDialog(QWidget *parent) : ModelTemplate(parent) {
            setWindowTitle("Model creation");

            QVBoxLayout *mainLayout = new QVBoxLayout(this);

            setupComboBox("Model");
            setupComboBox("Noise");
            setupComboBox("ML");

            setupLineEdit("Delta", "1.0", validator.get());
            setupLineEdit("Epochs", "1000", integer_validator.get());
            setupLineEdit("Step", "0.001", validator.get());
            setupLineEdit("Param.1", "0.0", validator.get());
            setupLineEdit("Param.2", "1.0", validator.get());
            setupLineEdit("Num.Feat.", "3", integer_validator.get());
            setupLineEdit("Path", "source.csv", nullptr);
            setupLineEdit("Target", "-1.2;2.7;3.5;4.78", coeffs_validator.get());
            
            setupLineEdit("Min.Noise", "35", integer_validator.get());
            setupLineEdit("Max.Noise", "50", integer_validator.get());
            setupLineEdit("ML.Param.1", "0.0", validator.get());
            setupLineEdit("ML.Param.2", "0.0", validator.get());
            setupLineEdit("Num.Exp.", "10", integer_validator.get());

            _info = std::make_unique<QPushButton>();
            _info->setText("Get info");

            QFormLayout *formLayout = new QFormLayout();

            formLayout->addRow("Model type:", _comboBoxes["Model"].get());
            formLayout->addRow("Delta:", _lineEdits["Delta"].get());
            formLayout->addRow("Epochs:", _lineEdits["Epochs"].get());
            formLayout->addRow("Learning Rate:", _lineEdits["Step"].get());

            formLayout->addRow("Noise type:", _comboBoxes["Noise"].get());
            formLayout->addRow("Parameter 1:", _lineEdits["Param.1"].get());
            formLayout->addRow("Parameter 2:", _lineEdits["Param.2"].get());
            formLayout->addRow("Path to data:", _lineEdits["Path"].get());
            formLayout->addRow("Target: ", _lineEdits["Target"].get());
            formLayout->addRow("Num. features:", _lineEdits["Num.Feat."].get());
            formLayout->addRow("Min noise:", _lineEdits["Min.Noise"].get());
            formLayout->addRow("Max noise:", _lineEdits["Max.Noise"].get());
            formLayout->addRow("ML model:", _comboBoxes["ML"].get());
            formLayout->addRow("ML Parameter 1:", _lineEdits["ML.Param.1"].get());
            formLayout->addRow("ML Parameter 2:", _lineEdits["ML.Param.2"].get());
            formLayout->addRow("Number of experiments", _lineEdits["Num.Exp."].get());
            formLayout->addRow("About distributions:", _info.get());

            mainLayout->addLayout(formLayout);

            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            mainLayout->addWidget(buttonBox);

            connect(buttonBox, &QDialogButtonBox::accepted, this, &ModelDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, this, &ModelDialog::reject);
            connect(_info.get(), &QPushButton::clicked, this, &ModelDialog::onInfoButtonPushed);
            connect(_comboBoxes["Model"].get(), &QComboBox::currentTextChanged, this, &ModelDialog::onModelChanged);
            onModelChanged(_comboBoxes["Model"].get()->currentText());
        }

        QStringList ModelDialog::getData() {
            auto path = _lineEdits["Path"]->text();
            if (!QFile::exists(path)) {
                createDialog(static_cast<QWidget*>(this->parent()), "Error", "File doesn\'t exist, field will be empty");
                path = "";
            }
            
            return {
                _comboBoxes["Model"]->currentText(),
                _lineEdits["Delta"]->text(),
                _lineEdits["Epochs"]->text(),
                _lineEdits["Step"]->text(),
                _comboBoxes["Noise"]->currentText(),
                _lineEdits["Param.1"]->text(),
                _lineEdits["Param.2"]->text(),
                _comboBoxes["ML"]->currentText(),
                _lineEdits["ML.Param.1"]->text(),
                _lineEdits["ML.Param.2"]->text(),
                path,
                _lineEdits["Num.Feat."]->text(),
                _lineEdits["Min.Noise"]->text(),
                _lineEdits["Max.Noise"]->text(),
                _lineEdits["Num.Exp."]->text(),
                _lineEdits["Target"]->text(),
            };
        }

        void ModelDialog::setupComboBox(const QString& name) {
            _comboBoxes[name] = std::make_unique<QComboBox>();
            if (name == "Model") {
                _comboBoxes[name]->addItems(modelsList);
            } else if (name == "Noise") {
                _comboBoxes[name]->addItems(distributionsList);
            } else {
                _comboBoxes[name]->addItems(mlModelsList);
            }
        }

        void ModelDialog::onInfoButtonPushed() {
            QDialog* infoDialog = new QDialog(this);
            infoDialog->setWindowTitle("Parameters");
            infoDialog->setAttribute(Qt::WA_DeleteOnClose);
            infoDialog->resize(static_cast<int>(Defaults::DIALOG_WIDTH) + 100, static_cast<int>(Defaults::DIALOG_WIDTH));

            QGridLayout *layout = new QGridLayout();
            layout->addWidget(getLabel("Distribution"), 0, 0);
            layout->addWidget(getLabel("Parameter 1"), 0, 1);
            layout->addWidget(getLabel("Parameter 2"), 0, 2);
            size_t distSize =  distributionsList.size();
            size_t mlSize = mlModelsList.size();
            for (int i = 0; i < distSize; ++i) {
                layout->addWidget(getLabel(distributionsList[i]), i+1, 0);
            }
            for (int i = 0; i < mlSize; ++i) {
                if (mlModelsList[i] != "None") {
                    layout->addWidget(getLabel(mlModelsList[i]), i + distSize + 1, 0);
                }
            }
            auto descrParameters = [&](std::string first, std::string second, int pos) {
                layout->addWidget(getLabel(QString::fromStdString(first)), pos, 1);
                layout->addWidget(getLabel(QString::fromStdString(second)), pos, 2);
            };
            descrParameters("Mean", "Deviation", 1);
            descrParameters("Deg. of freedom", "IGNORED", 2);
            descrParameters("Location", "Scale", 3);
            descrParameters("Location", "Scale", 4);
            descrParameters("IGNORED", "IGNORED", 5);
            descrParameters("", "", 6);
            descrParameters("K", "Contamination", 7);
            descrParameters("Gamma", "IGNORED", 8);
            descrParameters("Num. estimators", "Depth", 9);
            descrParameters("R", "Min cluster size", 10);



            infoDialog->setLayout(layout);
            infoDialog->show();
        }

        QLabel* ModelDialog::getLabel(const QString& text) {
            QLabel *label = new QLabel();
            label->setText(text);
            return label;
        }

        // names has to be enum;
        void ModelDialog::onModelChanged(const QString &modelType) {
            bool enableParams = (modelType != "LSM" && modelType != "LAD");
            bool enableThsDelta = (modelType != "THS");

            _lineEdits["Delta"]->setEnabled(enableParams && enableThsDelta);
            _lineEdits["Epochs"]->setEnabled(enableParams);
            _lineEdits["Step"]->setEnabled(enableParams);
        }
    } // namespace UI;
} // namespace CP;
