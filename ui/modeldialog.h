#ifndef MODELDIALOG_H
#define MODELDIALOG_H

#include "modeltemplate.h"

namespace CP {
    namespace UI {

        class ModelDialog : public ModelTemplate {
            Q_OBJECT

        public:
            explicit ModelDialog(QWidget *parent = nullptr);
            QStringList getData() override;

        private:
            void setupComboBox(const QString& name);
            QLabel* getLabel(const QString& text);
            std::unique_ptr<QPushButton> _info;

            const QStringList distributionsList = {"Normal", "Student", "Cauchy", "Laplace", "Scale"};
            const QStringList mlModelsList = {"None", "IForest", "DBSCAN", "KDE", "KNN"};
            const QStringList modelsList = {"LSM", "HUB", "TUK", "LAD", "THS"};

            std::unordered_map<QString, std::unique_ptr<QComboBox>> _comboBoxes;

        private slots:
            void onModelChanged(const QString &modelType);
            void onInfoButtonPushed();
        };
    } // namespace UI;
} // namespace CP;

#endif // MODELDIALOG_H
