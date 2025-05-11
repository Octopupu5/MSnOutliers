#ifndef MODELTEMPLATE_H
#define MODELTEMPLATE_H

#include "defaults.h"

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

#include <iostream>
#include <unordered_map>

namespace CP {
    namespace UI {

        class ModelTemplate : public QDialog {
            Q_OBJECT

        public:
            explicit ModelTemplate(QWidget *parent = nullptr) : QDialog(parent) {}
            virtual QStringList getData() = 0;

        protected:
            void setupLineEdit(const QString name, QString text, QValidator *validator);
            std::unordered_map<QString, std::unique_ptr<QLineEdit>> _lineEdits;
        };
    } // namespace UI;
} // namespace CP;

#endif // MODELTEMPLATE_HgenerateData
