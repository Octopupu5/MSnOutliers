#include "mainwindow.h"
#include "modeldialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

using json = nlohmann::json;
MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Regression analysis");
    resize(300, 150);

    QPushButton *createBtn = new QPushButton("Create model");
    QPushButton *dumpBtn = new QPushButton("Dump models");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createBtn);
    layout->addWidget(dumpBtn);
    layout->addStretch();

    connect(createBtn, &QPushButton::clicked, this, &MainWindow::openModelDialog);
    connect(dumpBtn, &QPushButton::clicked, this, &MainWindow::dumpModels);
}

void MainWindow::openModelDialog() {
    ModelDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QStringList result = dialog.getModelData();
        _models.push_back(std::move(result));
    }
}

void MainWindow::dumpModels() {
    std::vector<json> models_;
    for (auto& el : _models) {
        json tmp;
        assert(el.size() == 4 && "Malformed data");
        tmp[el[0].toStdString()] = {
                {"delta", el[1].toStdString()},
                {"eps",   el[2].toStdString()},
                {"lr",    el[3].toStdString()}
        };
        models_.push_back(std::move(tmp));
    }

    std::ofstream f("models.json");
    json j(models_);
    qDebug() << j.dump(4);
    f << j.dump(4);
    f.close();
}
