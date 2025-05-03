#ifndef MODELDIALOG_H
#define MODELDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include <iostream>

class ModelDialog : public QDialog {
    Q_OBJECT

public:
    explicit ModelDialog(QWidget *parent = nullptr);
    QStringList getModelData() const;

private:
    QLabel* getLabel(const QString& text);

    std::unique_ptr<QComboBox> _model;
    std::unique_ptr<QComboBox> _noise;
    std::unique_ptr<QComboBox> _mlmodel;

    std::unique_ptr<QLineEdit> _delta;
    std::unique_ptr<QLineEdit>  _epochs;
    std::unique_ptr<QLineEdit>  _lr;
    std::unique_ptr<QLineEdit>  _param1;
    std::unique_ptr<QLineEdit>  _param2;

    std::unique_ptr<QPushButton> _info;

    const QStringList distributionsList = {"Normal", "Student", "Cauchy", "Lognormal", "Laplace"};
    const QStringList mlModelsList = {"None", "IForest", "DBSCAN", "OCSVM"};
    const QStringList modelsList = {"LSM", "HUB", "TUK", "LAD", "THS"};

private slots:
    void onModelChanged(const QString &modelType);
    void onInfoButtonPushed();
};

#endif // MODELDIALOG_H
