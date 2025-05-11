#include "modeltemplate.h"

namespace CP {
    namespace UI {
        
        void createDialog(QWidget* parent, QString title, QString name);
        ModelTemplate::ModelTemplate(QWidget *parent) : QDialog(parent) {
            QRegularExpression re_int(R"(0|[1-9]\d*)");
            QRegularExpression re_coeffs(R"(^(\d+\.\d+)(;\d+\.\d+)*$)");
            QRegularExpression re(R"(^-?(0|[1-9]\d*)(\.\d{0,4})?$)");
            integer_validator = std::make_unique<QRegularExpressionValidator>(re_int, this);
            coeffs_validator = std::make_unique<QRegularExpressionValidator>(re_coeffs, this);
            validator = std::make_unique<QRegularExpressionValidator>(re, this);
        }

        void ModelTemplate::setupLineEdit(const QString name, QString text, QValidator *validator) {
            _lineEdits[name] = std::make_unique<QLineEdit>();
            _lineEdits[name]->setText(std::move(text));
            _lineEdits[name]->setValidator(validator);
        }
    } // namespace UI;
} // namespace CP;
