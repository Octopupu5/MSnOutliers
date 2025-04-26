#include "mainwindow.h"
#include "modeldialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QFile>
#include <QLabel>

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
    createDialog("Success", "Models dumped to models.json");
}

void MainWindow::runMethods() {
    std::string binary = std::string(PATH_TO_BINARY);
    std::string path = std::string(PATH_TO_OUTPUT) + "models.json";
    if (!QFile::exists(QString::fromStdString(path))) {
        createDialog("Error", "There is no models.json");
    } else {
        std::cout << binary + " " + path << std::endl;
        int res = std::system((binary + " " + path).c_str()); // NEED TO FIX THIS!!!!
        std::cout << res << std::endl;

        QList<QString> methods;
        for (const auto &model : _models) {
            methods.append(model[0]);
        }
        showImage(methods);
        _models.clear();
    }
}

void MainWindow::createDialog(QString title, QString message) {
    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(std::move(title));
    dialog->resize(300, 100);
    QTextEdit *text = new QTextEdit(std::move(dialog));
    text->setPlainText(message);
    text->setAlignment(Qt::AlignCenter);
    text->setReadOnly(true);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(text);
    dialog->setLayout(layout);
    dialog->show();
}

void MainWindow::showImage(const QList<QString> &methods) {
    QWidget *imageWindow = new QWidget();
    imageWindow->setWindowTitle("Models");
    imageWindow->setAttribute(Qt::WA_DeleteOnClose);
    imageWindow->resize(500, 500);

    QGridLayout *layout = new QGridLayout(imageWindow);

    int row = 0;
    int col = 0;
    for (const auto &method : methods) {
        std::string path = std::string(PATH_TO_PICTURES) + "out_" + method.toStdString() + ".png";
        if (QFile::exists(QString::fromStdString(path))) {
            QLabel *imageLabel = new QLabel();
            QPixmap pixmap(QString::fromStdString(path));
            imageLabel->setPixmap(pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setScaledContents(true);

            layout->addWidget(imageLabel, row, col);

            col++;
            if (col >= 3) {
                col = 0;
                row++;
            }
        } else {
            qDebug() << "Image not found for method" << method;
        }
    }

    imageWindow->show();
}
