#ifndef MODELDIALOG_H
#define MODELDIALOG_H

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

        class ModelDialog : public QDialog {
            Q_OBJECT

        public:
            explicit ModelDialog(QWidget *parent = nullptr);
            QStringList getModelData();

        private:
            QLabel* getLabel(const QString& text);
            void setupComboBox(const QString& name);
            void setupLineEdit(const QString name, QString text, QValidator *validator);
            std::unique_ptr<QPushButton> _info;

            const QStringList distributionsList = {"Normal", "Student", "Cauchy", "Lognormal", "Laplace"};
            const QStringList mlModelsList = {"None", "IForest", "DBSCAN", "OCSVM"};
            const QStringList modelsList = {"LSM", "HUB", "TUK", "LAD", "THS"};

            std::unordered_map<QString, std::unique_ptr<QComboBox>> _comboBoxes;
            std::unordered_map<QString, std::unique_ptr<QLineEdit>> _lineEdits;

        private slots:
            void onModelChanged(const QString &modelType);
            void onInfoButtonPushed();
        };
    } // namespace UI;
} // namespace CP;

#endif // MODELDIALOG_H
