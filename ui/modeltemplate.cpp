#include "modeltemplate.h"

namespace CP {
    namespace UI {
        
        void createDialog(QWidget* parent, QString title, QString name);

        void ModelTemplate::setupLineEdit(const QString name, QString text, QValidator *validator) {
            _lineEdits[name] = std::make_unique<QLineEdit>();
            _lineEdits[name]->setText(std::move(text));
            _lineEdits[name]->setValidator(validator);
        }
    } // namespace UI;
} // namespace CP;
