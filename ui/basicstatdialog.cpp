#include "basicstatdialog.h"

namespace CP {
    namespace UI {
        BasicStatDialog::BasicStatDialog(QWidget *parent) : ModelTemplate(parent) {
            setupLineEdit("Path", "build/ui/source.csv", nullptr);
            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            QFormLayout *formLayout = new QFormLayout();
            formLayout->addRow("Path to data", _lineEdits["Path"].get());

            mainLayout->addLayout(formLayout);
            mainLayout->addWidget(buttonBox);
            resize(static_cast<int>(Defaults::DIALOG_WIDTH) + 100, static_cast<int>(Defaults::DIALOG_HEIGHT));
            setWindowTitle("Basic analysis");

            connect(buttonBox, &QDialogButtonBox::accepted, this, &BasicStatDialog::accept);
            connect(buttonBox, &QDialogButtonBox::rejected, this, &BasicStatDialog::reject);
        }

        QStringList BasicStatDialog::getData() {
            return {_lineEdits["Path"]->text()};
        }
    } // namespace UI;
} // namespace CP;