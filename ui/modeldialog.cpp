#include "modeldialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
ModelDialog::ModelDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Model creation");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    _model = std::make_unique<QComboBox>();
    _model->addItems(modelsList);
    _noise = std::make_unique<QComboBox>();
    _noise->addItems(distributionsList);

    QRegularExpression re(R"(^-?(0|[1-9]\d*)(\.\d{0,4})?$)");
    QRegularExpression re_int(R"(0|[1-9]\d*)");
    QValidator *validator = new QRegularExpressionValidator(re, this);
    QValidator *integer_validator = new QRegularExpressionValidator(re_int, this);

    _delta = std::make_unique<QLineEdit>();
    _delta->setText("1.0");
    _epochs = std::make_unique<QLineEdit>();
    _epochs->setText("1000");
    _lr = std::make_unique<QLineEdit>();
    _lr->setText("0.001");

    _param1 = std::make_unique<QLineEdit>();
    _param1->setText("0.0");
    _param2 = std::make_unique<QLineEdit>();
    _param2->setText("1.0");

    _delta->setValidator(validator);
    _epochs->setValidator(integer_validator);
    _lr->setValidator(validator);
    _param1->setValidator(validator);
    _param2->setValidator(validator);
    _info = std::make_unique<QPushButton>();
    _info->setText("Get info");

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Model type:", _model.get());
    formLayout->addRow("Delta:", _delta.get());
    formLayout->addRow("Epochs:", _epochs.get());
    formLayout->addRow("Learning Rate:", _lr.get());

    formLayout->addRow("Noise type:", _noise.get());
    formLayout->addRow("Parameter 1:", _param1.get());
    formLayout->addRow("Parameter 2:", _param2.get());
    formLayout->addRow("About distributions:", _info.get());

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ModelDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ModelDialog::reject);
    connect(_info.get(), &QPushButton::clicked, this, &ModelDialog::onInfoButtonPushed);
    connect(_model.get(), &QComboBox::currentTextChanged, this, &ModelDialog::onModelChanged);
    onModelChanged(_model->currentText());
}

QStringList ModelDialog::getModelData() const {
    return {
        _model->currentText(),
        _delta->text(),
        _epochs->text(),
        _lr->text(),
        _noise->currentText(),
        _param1->text(),
        _param2->text()    
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
    // add more;
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

    _delta->setEnabled(enableParams && enableThsDelta);
    _epochs->setEnabled(enableParams);
    _lr->setEnabled(enableParams);
}
