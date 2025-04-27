#ifndef MODELDIALOG_H
#define MODELDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

#include <iostream>

class ModelDialog : public QDialog {
    Q_OBJECT

public:
    explicit ModelDialog(QWidget *parent = nullptr);

    QStringList getModelData() const;

private:
    std::unique_ptr<QComboBox> _model;
    std::unique_ptr<QComboBox> _noise;

    std::unique_ptr<QLineEdit> _delta;
    std::unique_ptr<QLineEdit>  _epochs;
    std::unique_ptr<QLineEdit>  _lr;
    std::unique_ptr<QLineEdit>  _param1;
    std::unique_ptr<QLineEdit>  _param2;

private slots:
    void onModelChanged(const QString &modelType);
};

#endif // MODELDIALOG_H
