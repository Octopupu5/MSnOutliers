#include "modeldialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

ModelDialog::ModelDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Model creation");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    setupComboBox("Model");
    setupComboBox("Noise");
    setupComboBox("MLModel");

    QRegularExpression re(R"(^-?(0|[1-9]\d*)(\.\d{0,4})?$)");
    QRegularExpression re_int(R"(0|[1-9]\d*)");
    QValidator *validator = new QRegularExpressionValidator(re, this);
    QValidator *integer_validator = new QRegularExpressionValidator(re_int, this);

    setupLineEdit("Delta", "1.0", validator);
    setupLineEdit("Epochs", "1000", integer_validator);
    setupLineEdit("Step", "0.001", validator);
    setupLineEdit("Param.1", "0.0", validator);
    setupLineEdit("Param.2", "1.0", validator);

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
    formLayout->addRow("About distributions:", _info.get());

    formLayout->addRow("Denoising ML model:", _comboBoxes["MLModel"].get());

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ModelDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ModelDialog::reject);
    connect(_info.get(), &QPushButton::clicked, this, &ModelDialog::onInfoButtonPushed);
    connect(_comboBoxes["Model"].get(), &QComboBox::currentTextChanged, this, &ModelDialog::onModelChanged);
    onModelChanged(_comboBoxes["Model"].get()->currentText());
}

void ModelDialog::setupLineEdit(const QString name, QString text, QValidator *validator) {
    _lineEdits[name] = std::make_unique<QLineEdit>();
    _lineEdits[name]->setText(std::move(text));
    _lineEdits[name]->setValidator(validator);
}

void ModelDialog::setupComboBox(const QString& name) {
    _comboBoxes[name] = std::make_unique<QComboBox>();
    if (name == "Model") {
        _comboBoxes[name]->addItems(modelsList);
    } else if (name == "Noise") {
        _comboBoxes[name]->addItems(distributionsList);
    } else if (name == "MLModel") {
        _comboBoxes[name]->addItems(mlModelsList);
    }
}

QStringList ModelDialog::getModelData() {
    return {
        _comboBoxes["Model"]->currentText(),
        _lineEdits["Delta"]->text(),
        _lineEdits["Epochs"]->text(),
        _lineEdits["Step"]->text(),
        _comboBoxes["Noise"]->currentText(),
        _lineEdits["Param.1"]->text(),
        _lineEdits["Param.2"]->text(),
        _comboBoxes["MLModel"]->currentText()
    };
}

void ModelDialog::onInfoButtonPushed() {
    QDialog* infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("Distributions\' parameters");
    infoDialog->setAttribute(Qt::WA_DeleteOnClose);
    infoDialog->resize(300, 300);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(getLabel("Distribution"), 0, 0);
    layout->addWidget(getLabel("Parameter 1"), 0, 1);
    layout->addWidget(getLabel("Parameter 2"), 0, 2);
    for (int i = 0; i < distributionsList.size(); ++i) {
        layout->addWidget(getLabel(distributionsList[i]), i+1, 0);
    }
    layout->addWidget(getLabel("Mean"), 1, 1);
    layout->addWidget(getLabel("Deviation"), 1, 2);

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
