#ifndef MODELDIALOG_H
#define MODELDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

class ModelDialog : public QDialog {
    Q_OBJECT

public:
    explicit ModelDialog(QWidget *parent = nullptr);

    QStringList getModelData() const;

private:
    QComboBox *_model;
    QLineEdit *_delta;
    QLineEdit *_epochs;
    QLineEdit *_lr;

private slots:
    void onModelChanged(const QString &modelType);
};

#endif // MODELDIALOG_H
