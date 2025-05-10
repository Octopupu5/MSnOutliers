#include "mainwindow.h"

namespace CP {
    namespace UI {
        using json = nlohmann::json;

        void createDialog(QWidget* parent, QString title, QString message) {
            QDialog *dialog = new QDialog(parent);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->setWindowTitle(std::move(title));
            dialog->resize(300, 100);
            QLabel *label = new QLabel(parent);
            label->setAlignment(Qt::AlignCenter);
            label->setText(message);
            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->addWidget(label);
            dialog->setLayout(layout);
            dialog->show();
        }

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
                const QStringList headers = {"Model", "Delta", "Epochs", "Learn. Rate", "(N) Type", "(N) Param. 1", "(N) Param. 2", "ML", "Data", "Num. features"};
                if (section >= 0 && section < headers.size()) {
                    return headers.at(section);
                }
            }

            return QVariant();
        }

        MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
            setWindowTitle("Regression analysis");
            resize(1050, 300);

            QPushButton *createBtn = new QPushButton("Create model");
            QPushButton *dumpBtn = new QPushButton("Dump models");
            QPushButton *runBtn = new QPushButton("Run on models");
            QPushButton *genBtn = new QPushButton("Generate data");


            QVBoxLayout *layout = new QVBoxLayout(this);
            model = new ModelTable();
            model->setData(_models);
            auto view = new QTableView();
            view->setModel(model);

            layout->addWidget(createBtn);
            layout->addWidget(dumpBtn);
            layout->addWidget(runBtn);
            layout->addWidget(genBtn);
            layout->addWidget(view);

            layout->addStretch();

            connect(createBtn, &QPushButton::clicked, this, &MainWindow::openModelDialog);
            connect(dumpBtn, &QPushButton::clicked, this, &MainWindow::dumpModels);
            connect(runBtn, &QPushButton::clicked, this, &MainWindow::runMethods);
            connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateData);
        }

        void MainWindow::openModelDialog() {
            ModelDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                QStringList result = dialog.getData();
                _models.push_back(std::move(result));
                model->setData(_models);
            }
        }

        void MainWindow::generateData() {
            GenerateDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                auto result = dialog.getData();
                int feat = (result[0].toStdString().empty() ? 3 : std::stoi(result[0].toStdString()));
                int samp = (result[1].toStdString().empty() ? 100 : std::stoi(result[1].toStdString()));
                generate(feat, samp);
            }
        }

        void MainWindow::generate(int numFeatures, int numSamples) {
            std::ostringstream s;
            for (int i = 0; i < numSamples; ++i) {
                for (int j = 0; j < numFeatures; ++j) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(1.0, 100.0);
                    double random_double = dis(gen);
                    s << random_double;
                    if (j != numFeatures - 1) {
                        s << ";";
                    }
                }
                if (i != numSamples - 1) {
                    s << "\n";
                }
            }
            std::ofstream f("sample.csv");
            std::cout << s.str();
            f << s.str();
            f.close();
        }

        void MainWindow::dumpModels() {
            std::vector<json> models_;
            std::string path = std::string(PATH_TO_OUTPUT);
            for (auto& el : _models) {
                json tmp;
                assert(el.size() == 10 && "Malformed data");
                auto deltaStr    = (el[1].toStdString().empty() ? "1" : el[1].toStdString());
                auto epsStr      = (el[2].toStdString().empty() ? "1000" : el[2].toStdString());
                auto lrStr       = (el[3].toStdString().empty() ? "0.001" : el[3].toStdString());
                auto param1Str   = (el[5].toStdString().empty() ? "0.0" : el[5].toStdString());
                auto param2Str   = (el[6].toStdString().empty() ? "1.0" : el[6].toStdString());
                auto numFeatStr   = (el[9].toStdString().empty() ? "3" : el[9].toStdString());

                tmp[el[0].toStdString()] = {
                    {"delta", std::stod(deltaStr)},
                    {"eps",   std::stoi(epsStr)},
                    {"lr",    std::stod(lrStr)},
                    {"noise", {{"type", el[4].toStdString()},
                            {"param1", std::stod(param1Str)},
                            {"param2", std::stod(param2Str)}
                            }
                    },
                    {"mlmodel", el[7].toStdString()},
                    {"path", el[8].toStdString()},
                    {"num_feat", std::stoi(numFeatStr)}
                };
                models_.push_back(std::move(tmp));
            }

            std::ofstream f(path + "models.json");
            json j;
            j["models"] = std::move(models_);
            f << j.dump(4);
            f.close();
            createDialog(this, "Success", "Models dumped to file: models.json");
        }

        void MainWindow::runMethods() {
            std::ostringstream s;
            s << std::string(PATH_TO_OUTPUT) << "models.json";
            std::string binary = std::string(PATH_TO_BINARY);
            std::string path = s.str();
            s.clear();
            if (!QFile::exists(QString::fromStdString(path))) {
                createDialog(this, "Error", "No such file: models.json");
            } else {
                std::ostringstream ss;
                ss << binary << " " << path;
                int res = std::system(ss.str().c_str());

                QList<QPair<QString, QString>> methods;
                for (const auto &model : _models) {
                    methods.append({model[0], model[7]});
                }
                showImage(methods);
                _models.clear();
                model->setData(_models);
            }
        }

        void MainWindow::showImage(const QList<QPair<QString, QString>>& methods) {
            QWidget *imageWindow = new QWidget();
            imageWindow->setWindowTitle("Models");
            imageWindow->setAttribute(Qt::WA_DeleteOnClose);
            imageWindow->resize(800, 600);

            QScrollArea *scrollArea = new QScrollArea(imageWindow);
            scrollArea->setWidgetResizable(true);
            scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            
            QWidget *scrollContent = new QWidget();
            QVBoxLayout *mainLayout = new QVBoxLayout(scrollContent);
            mainLayout->setAlignment(Qt::AlignHCenter);
            mainLayout->setSpacing(20);
            QMap<QString, QList<QString>> methodGroups;
            for (const auto &method : methods) {
                if (!methodGroups.contains(method.first)) {
                    methodGroups[method.first] = QList<QString>();
                }
                methodGroups[method.first].append(method.second);
            }
            
            for (auto it = methodGroups.constBegin(); it != methodGroups.constEnd(); ++it) {
                const QString &modelName = it.key();
                int numMethods = it.value().size();
                int numGroups = (numMethods + 3) / 4;
                for (int groupIdx = 0; groupIdx < numGroups; ++groupIdx) {
                    std::ostringstream s;
                    s << std::string(PATH_TO_PICTURES) << "out_" << modelName.toStdString() << "_group" << groupIdx << ".png";
                    QString path = QString::fromStdString(s.str());
                    
                    if (QFile::exists(path)) {
                        QWidget *graphContainer = new QWidget();
                        graphContainer->setFixedWidth(700);
                        QVBoxLayout *containerLayout = new QVBoxLayout(graphContainer);
                        QLabel *titleLabel = new QLabel(modelName + " (Group " + QString::number(groupIdx + 1) + ")");
                        titleLabel->setAlignment(Qt::AlignCenter);
                        titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
                        containerLayout->addWidget(titleLabel);
                        QWidget *imageContainer = new QWidget();
                        imageContainer->setFixedSize(680, 500);
                        QVBoxLayout *imageLayout = new QVBoxLayout(imageContainer);
                        imageLayout->setContentsMargins(0, 0, 0, 0);
                        
                        QLabel *imageLabel = new QLabel();
                        imageLabel->setAlignment(Qt::AlignCenter);
                        QPixmap pixmap(path);
                        
                        imageLabel->setPixmap(pixmap.scaled(670, 490, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                        imageLabel->setMinimumSize(670, 490);
                        imageLabel->setMaximumSize(670, 490);
                        
                        imageLayout->addWidget(imageLabel);
                        containerLayout->addWidget(imageContainer);
                        mainLayout->addWidget(graphContainer);
                    }
                }
            }
            
            if (mainLayout->count() == 0) {
                QLabel *noImagesLabel = new QLabel("No images found for selected models");
                noImagesLabel->setAlignment(Qt::AlignCenter);
                noImagesLabel->setStyleSheet("font-size: 16px; color: gray;");
                mainLayout->addWidget(noImagesLabel);
            }
            
            scrollContent->setLayout(mainLayout);
            scrollArea->setWidget(scrollContent);
            
            QVBoxLayout *windowLayout = new QVBoxLayout(imageWindow);
            windowLayout->setContentsMargins(0, 0, 0, 0);
            windowLayout->addWidget(scrollArea);
            
            imageWindow->show();
        }
    } // namespace UI;
} // namespace CP;
