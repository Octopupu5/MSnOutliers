#include "generatedialog.h"

namespace CP {
    namespace UI {

        GenerateDialog::GenerateDialog(QWidget *parent) : ModelTemplate(parent) {
            QRegularExpression re_int(R"(0|[1-9]\d*)");
            QValidator *integer_validator = new QRegularExpressionValidator(re_int, this);
            setupLineEdit("Features", "3", integer_validator);
            setupLineEdit("Samples", "100", integer_validator);

            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QFormLayout *formLayout = new QFormLayout();
            formLayout->addRow("Num. features", _lineEdits["Features"].get());
            formLayout->addRow("Num. samples", _lineEdits["Samples"].get());

            mainLayout->addLayout(formLayout);
            mainLayout->addWidget(buttonBox);
            resize(200, 100);
            setWindowTitle("Generate data");

            connect(buttonBox, &QDialogButtonBox::accepted, this, &GenerateDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, this, &GenerateDialog::reject);
        }

        QStringList GenerateDialog::getData() {
            return {_lineEdits["Features"]->text(), _lineEdits["Samples"]->text()};
        }
    } // namespace UI;
} // namespace CP;