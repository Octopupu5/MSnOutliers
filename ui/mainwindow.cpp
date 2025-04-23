#include "mainwindow.h"
#include "modeldialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QFile>

using json = nlohmann::json;
MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Regression analysis");
    resize(300, 150);

    QPushButton *createBtn = new QPushButton("Create model");
    QPushButton *dumpBtn = new QPushButton("Dump models");
    QPushButton *runBtn = new QPushButton("Run on models");


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createBtn);
    layout->addWidget(dumpBtn);
    layout->addWidget(runBtn);
    layout->addStretch();

    connect(createBtn, &QPushButton::clicked, this, &MainWindow::openModelDialog);
    connect(dumpBtn, &QPushButton::clicked, this, &MainWindow::dumpModels);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runMethods);
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
    std::string path = std::string(PATH_TO_OUTPUT);
    for (auto& el : _models) {
        json tmp;
        assert(el.size() == 4 && "Malformed data");
        auto deltaStr = (el[1].toStdString().empty() ? "1" : el[1].toStdString());
        auto epsStr = (el[2].toStdString().empty() ? "1000" : el[2].toStdString());
        auto lrStr = (el[3].toStdString().empty() ? "0.001" : el[3].toStdString());

        tmp[el[0].toStdString()] = {
                {"delta", std::stod(deltaStr)},
                {"eps",   std::stod(epsStr)},
                {"lr",    std::stod(lrStr)}
        };
        models_.push_back(std::move(tmp));
    }

    std::ofstream f(path + "models.json");
    json j(models_);
    f << j.dump(4);
    f.close();
}

void MainWindow::runMethods() {
    std::string binary = std::string(PATH_TO_BINARY);
    std::string path = std::string(PATH_TO_OUTPUT) + "models.json";
    // assert(QFile::exists(QString::fromStdString(path)) && "There is no models.json");
    if (!QFile::exists(QString::fromStdString(path))) {
        QDialog *errorWindow = new QDialog(this);
        errorWindow->setWindowTitle("Error");
        errorWindow->resize(300, 100);
        QTextEdit *text = new QTextEdit(errorWindow);
        text->setPlainText("There is no models.json");
        text->setAlignment(Qt::AlignCenter);
        text->setReadOnly(true);
        QVBoxLayout *layout = new QVBoxLayout(errorWindow);
        layout->addWidget(text);
        errorWindow->setLayout(layout);
        errorWindow->show();
    } else {
        std::cout << binary + " " + path << std::endl;
        int res = std::system((binary + " " + path).c_str()); // NEED TO FIX THIS!!!!
        std::cout << res << std::endl;
    }
}
