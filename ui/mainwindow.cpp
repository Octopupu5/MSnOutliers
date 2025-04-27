#include "mainwindow.h"
#include "modeldialog.h"
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include <QFile>
#include <QLabel>

using json = nlohmann::json;

void ModelTable::setData(const QList<QStringList> &data) {
    beginResetModel();
    _data = data;
    endResetModel();
}

int ModelTable::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return _data.size();
}

int ModelTable::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return _data.isEmpty() ? 0 : _data.first().size();
}

QVariant ModelTable::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const QStringList &row = _data.at(index.row());
    if (index.column() >= row.size()) {
        return QVariant();
    }

    return row.at(index.column());
}

QVariant ModelTable::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        const QStringList headers = {"Model", "Delta", "Epochs", "Learn. Rate", "(N) Type", "(N) Param. 1", "(N) Param. 2"};
        if (section >= 0 && section < headers.size()) {
            return headers.at(section);
        }
    }

    return QVariant();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Regression analysis");
    resize(750, 300);

    QPushButton *createBtn = new QPushButton("Create model");
    QPushButton *dumpBtn = new QPushButton("Dump models");
    QPushButton *runBtn = new QPushButton("Run on models");


    QVBoxLayout *layout = new QVBoxLayout(this);
    model = new ModelTable();
    model->setData(_models);
    auto view = new QTableView();
    view->setModel(model);

    layout->addWidget(createBtn);
    layout->addWidget(dumpBtn);
    layout->addWidget(runBtn);
    layout->addWidget(view);

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
        model->setData(_models);
    }
}

void MainWindow::dumpModels() {
    std::vector<json> models_;
    std::string path = std::string(PATH_TO_OUTPUT);
    for (auto& el : _models) {
        json tmp;
        assert(el.size() == 7 && "Malformed data");
        auto deltaStr    = (el[1].toStdString().empty() ? "1" : el[1].toStdString());
        auto epsStr      = (el[2].toStdString().empty() ? "1000" : el[2].toStdString());
        auto lrStr       = (el[3].toStdString().empty() ? "0.001" : el[3].toStdString());
        auto param1Str   = (el[5].toStdString().empty() ? "0.0" : el[5].toStdString());
        auto param2Str   = (el[6].toStdString().empty() ? "1.0" : el[6].toStdString());

        tmp[el[0].toStdString()] = {
            {"delta", std::stod(deltaStr)},
            {"eps",   std::stoi(epsStr)},
            {"lr",    std::stod(lrStr)},
            {"noise", {{"type", el[4].toStdString()},
                       {"param1", std::stod(param1Str)},
                       {"param2", std::stod(param2Str)}
                      }
            }
        };
        models_.push_back(std::move(tmp));
    }

    std::ofstream f(path + "models.json");
    json j;
    j["models"] = std::move(models_);
    f << j.dump(4);
    f.close();
    createDialog("Success", "Models dumped to file: models.json");
}

void MainWindow::runMethods() {
    std::string binary = std::string(PATH_TO_BINARY);
    std::string path = std::string(PATH_TO_OUTPUT) + "models.json";
    if (!QFile::exists(QString::fromStdString(path))) {
        createDialog("Error", "No such file: models.json");
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
        model->setData(_models);
    }
}

void MainWindow::createDialog(QString title, QString message) {
    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(std::move(title));
    dialog->resize(300, 100);
    QLabel *label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    label->setText(message);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(label);
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
