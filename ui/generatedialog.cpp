#include "generatedialog.h"

namespace CP {
    namespace UI {

        GenerateDialog::GenerateDialog(QWidget *parent) : ModelTemplate(parent) {
            setupLineEdit("Min", "1", integer_validator.get());
            setupLineEdit("Max", "100", integer_validator.get());
            setupLineEdit("Features", "3", integer_validator.get());
            setupLineEdit("Samples", "100", integer_validator.get());
            setupLineEdit("Coefficients", "", coeffs_validator.get());

            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QFormLayout *formLayout = new QFormLayout();
            formLayout->addRow("Minimum", _lineEdits["Min"].get());
            formLayout->addRow("Maximum", _lineEdits["Max"].get());
            formLayout->addRow("Num. features", _lineEdits["Features"].get());
            formLayout->addRow("Num. samples", _lineEdits["Samples"].get());
            formLayout->addRow("Coefficients", _lineEdits["Coefficients"].get());


            mainLayout->addLayout(formLayout);
            mainLayout->addWidget(buttonBox);
            resize(static_cast<int>(Defaults::DIALOG_WIDTH) + 100, static_cast<int>(Defaults::DIALOG_HEIGHT));
            setWindowTitle("Generate data");

            connect(buttonBox, &QDialogButtonBox::accepted, this, &GenerateDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, this, &GenerateDialog::reject);
        }

        QStringList GenerateDialog::getData() {
            return {_lineEdits["Features"]->text(), 
                    _lineEdits["Samples"]->text(),
                    _lineEdits["Coefficients"]->text(),
                    _lineEdits["Min"]->text(),
                    _lineEdits["Max"]->text()};
        }
    } // namespace UI;
} // namespace CP;