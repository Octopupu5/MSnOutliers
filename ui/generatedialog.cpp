#include "generatedialog.h"

namespace CP {
    namespace UI {

        GenerateDialog::GenerateDialog(QWidget *parent) : QDialog(parent) {
            _numFeatures = std::make_unique<QLineEdit>();
            QRegularExpression re_int(R"(0|[1-9]\d*)");
            QValidator *integer_validator = new QRegularExpressionValidator(re_int, this);
            _numFeatures->setValidator(integer_validator);

            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QFormLayout *formLayout = new QFormLayout();
            formLayout->addRow("Num. features", _numFeatures.get());

            mainLayout->addLayout(formLayout);
            mainLayout->addWidget(buttonBox);
            resize(200, 100);
            setWindowTitle("Generate data");

            connect(buttonBox, &QDialogButtonBox::accepted, this, &GenerateDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, this, &GenerateDialog::reject);
        }
    } // namespace UI;
} // namespace CP;