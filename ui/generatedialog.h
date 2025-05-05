#ifndef GENERATE_DIALOG_H
#define GENERATE_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QFile>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>

#include <unordered_map>

namespace CP {
    namespace UI {
        class GenerateDialog : public QDialog {
            Q_OBJECT

        public:
            explicit GenerateDialog(QWidget *parent = nullptr);
            
        private:
            std::unique_ptr<QLineEdit> _numFeatures;
        };
    } // namespace UI;
} // namespace CP;

#endif
