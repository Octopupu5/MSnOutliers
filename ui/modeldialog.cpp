#include "modeldialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
ModelDialog::ModelDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Model creation");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _model = new QComboBox();
    _model->addItems({"LSM", "HUB", "TUK", "LAD", "THS"});
    QRegularExpression re(R"(^-?(0|[1-9]\d*)(\.\d{0,4})?$)");
    QValidator *validator = new QRegularExpressionValidator(re, this);

    _delta = new QLineEdit();
    _epochs = new QLineEdit();
    _lr = new QLineEdit();
    _delta->setValidator(validator);
    _epochs->setValidator(validator);
    _lr->setValidator(validator);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Model type:", _model);
    formLayout->addRow("Delta:", _delta);
    formLayout->addRow("Epochs:", _epochs);
    formLayout->addRow("Learning Rate:", _lr);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ModelDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ModelDialog::reject);
    connect(_model, &QComboBox::currentTextChanged, this, &ModelDialog::onModelChanged);
    onModelChanged(_model->currentText());
}

QStringList ModelDialog::getModelData() const {
    return {
        _model->currentText(),
        _delta->text(),
        _epochs->text(),
        _lr->text()
    };
}
// names has to be enum;
void ModelDialog::onModelChanged(const QString &modelType) {
    bool enableParams = (modelType != "LSM" && modelType != "LAD");
    bool enableThsDelta = (modelType != "THS");

    _delta->setEnabled(enableParams && enableThsDelta);
    _epochs->setEnabled(enableParams);
    _lr->setEnabled(enableParams);
}
