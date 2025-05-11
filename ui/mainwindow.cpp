#include "mainwindow.h"

namespace CP {
    namespace UI {
        using json = nlohmann::json;

        void createDialog(QWidget* parent, QString title, QString message) {
            QDialog *dialog = new QDialog(parent);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->setWindowTitle(std::move(title));
            dialog->resize(static_cast<int>(Defaults::DIALOG_WIDTH), static_cast<int>(Defaults::DIALOG_HEIGHT));
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
                const QStringList headers = {"Model", "Delta", "Epochs", "Learn. Rate", "(N) Type", "(N) Param. 1", 
                                             "(N) Param. 2", "ML", "(ML) Param. 1", "(ML) Param. 2", "Data", "Num. features",
                                             "Min. Noise", "Max. Noise", "Num. Exp.", "Target"};
                if (section >= 0 && section < headers.size()) {
                    return headers.at(section);
                }
            }

            return QVariant();
        }

        MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
            setWindowTitle("Regression analysis");
            resize(static_cast<int>(Defaults::MAIN_WINDOW_WIDTH), static_cast<int>(Defaults::MAIN_WINDOW_HEIGHT));

            QPushButton *createBtn = new QPushButton("Create model");
            QPushButton *dumpBtn = new QPushButton("Dump models");
            QPushButton *runBtn = new QPushButton("Run on models");
            QPushButton *genBtn = new QPushButton("Generate data");
            QPushButton *statBtn = new QPushButton("Calculate statistics");


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
            layout->addWidget(statBtn);

            layout->addStretch();

            connect(createBtn, &QPushButton::clicked, this, &MainWindow::openModelDialog);
            connect(dumpBtn, &QPushButton::clicked, this, &MainWindow::dumpModels);
            connect(runBtn, &QPushButton::clicked, this, &MainWindow::runMethods);
            connect(genBtn, &QPushButton::clicked, this, &MainWindow::generateData);
            connect(statBtn, &QPushButton::clicked, this, &MainWindow::getBasicStatistics);
        }

        void MainWindow::openModelDialog() {
            ModelDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                QStringList result = dialog.getData();
                _models.push_back(std::move(result));
                model->setData(_models);
            }
        }

        std::vector<double> MainWindow::parseCoefficients(std::string coeffs) {
            std::vector<double> res;
            std::stringstream ss(coeffs);
            std::string num;
            while (std::getline(ss, num, ';')) {
                res.push_back(num.empty() ? 0 : std::stod(num));
            }
            return res;
        }

        void MainWindow::generateData() {
            GenerateDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                auto result = dialog.getData();
                int feat = (result[0].toStdString().empty() ? static_cast<int>(Defaults::FEATURES) : std::stoi(result[0].toStdString()));
                int samp = (result[1].toStdString().empty() ? static_cast<int>(Defaults::SAMPLES) : std::stoi(result[1].toStdString()));
                int min = (result[3].toStdString().empty() ? static_cast<int>(Defaults::GEN_MIN) : std::stoi(result[3].toStdString()));
                int max = (result[4].toStdString().empty() ? static_cast<int>(Defaults::GEN_MAX) : std::stoi(result[4].toStdString()));
                generate(min, max, feat, samp, std::move(parseCoefficients(std::move(result[2].toStdString()))));
            }
        }

        void MainWindow::getBasicStatistics() {
            BasicStatDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                auto result = dialog.getData();
                if (QFile::exists(result[0])) {
                    std::vector<double> targets;
                    double sum = 0, sum_squares = 0;
                    std::ifstream f(result[0].toStdString());
                    std::string currentObject;
                    while (std::getline(f, currentObject)) {
                        auto curLen = currentObject.size(), i = curLen - 1;
                        while (i >= 0 && currentObject[i] != ';') {
                            --i;
                        }
                        try { 
                            double target = std::stod(std::string(currentObject.begin() + i + 1, currentObject.end()));
                            targets.push_back(target);
                            sum += target;
                            sum_squares += (target * target);
                        } catch (...) {
                            createDialog(this, "Error", "Error parsing file, try again");
                        }
                    }
                    f.close();
                    std::sort(targets.begin(), targets.end());
                    auto len = targets.size();
                    double avg = sum/len;
                    std::ostringstream s;
                    s << "Max: " << targets[len-1] << "\n Min: " << targets[0] << "\n";
                    s << "Average: " << avg << "\nMedian: " << targets[len/2] << "\n";
                    s << "Deviation: " << sum_squares/len - avg * avg;
                    createDialog(this, "Statistics", QString::fromStdString(s.str())); 
                } else {
                    createDialog(this, "Error", "No such file");
                }
            }
        }

        void MainWindow::generate(int min, int max, int numFeatures, int numSamples, std::vector<double> coeffs) {
            if (coeffs.size() != numFeatures + 1) {
                createDialog(this, "Error", "Not enough coefficients, try again");
                return;
            }
            std::ostringstream s;
            std::string sample = "";
            for (int i = 0; i < numSamples; ++i) {
                double target = coeffs[0];
                for (int j = 0; j < numFeatures; ++j) {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(min, max);
                    double random_double = dis(gen);
                    s << random_double << ";";
                    target += random_double * coeffs[j+1];
                }
                s << target;
                if (i != numSamples - 1) {
                    s << "\n";
                }
                if (i == static_cast<int>(Defaults::GEN_DISPLAY)) {
                    sample = s.str();
                }
            }
            if (sample.empty()) {
                sample = s.str();
            }
            std::ofstream f("sample.csv");
            f << s.str();
            s.clear();
            f.close();
            std::ostringstream ss;
            ss << "Data dumped to sample.csv\nSample:\n" << sample;
            createDialog(this, "Success", QString::fromStdString(ss.str()));
        }

        void MainWindow::dumpModels() {
            std::vector<json> models_;
            std::string path = std::string(PATH_TO_OUTPUT);
            for (auto& el : _models) {
                json tmp;
                assert(el.size() == 16 && "Malformed data");
                auto deltaStr      = (el[1].toStdString().empty() ? "1" : el[1].toStdString());
                auto epsStr        = (el[2].toStdString().empty() ? "1000" : el[2].toStdString());
                auto lrStr         = (el[3].toStdString().empty() ? "0.001" : el[3].toStdString());
                auto param1Str     = (el[5].toStdString().empty() ? "0.0" : el[5].toStdString());
                auto param2Str     = (el[6].toStdString().empty() ? "1.0" : el[6].toStdString());
                auto mlParam1Str   = (el[8].toStdString().empty() ? "0.0" : el[8].toStdString());
                auto mlParam2Str   = (el[9].toStdString().empty() ? "0.0" : el[9].toStdString());
                auto numFeatStr    = (el[11].toStdString().empty() ? "3" : el[11].toStdString());
                auto minNoiseStr   = (el[12].toStdString().empty() ? "10" : el[12].toStdString());
                auto maxNoiseStr   = (el[13].toStdString().empty() ? "50" : el[13].toStdString());
                auto numExpStr     = (el[14].toStdString().empty() ? "10" : el[14].toStdString());

                tmp[el[0].toStdString()] = {
                    {"delta", std::stod(deltaStr)},
                    {"eps",   std::stoi(epsStr)},
                    {"lr",    std::stod(lrStr)},
                    {"noise", {{"type", el[4].toStdString()},
                            {"param1", std::stod(param1Str)},
                            {"param2", std::stod(param2Str)}
                            }
                    },
                    {"mlmodel", {{"type", el[7].toStdString()},
                            {"param1", std::stod(mlParam1Str)},
                            {"param2", std::stod(mlParam2Str)}
                            }
                    },
                    {"path", el[10].toStdString()},
                    {"num_feat", std::stoi(numFeatStr)},
                    {"min_noise", std::stoi(minNoiseStr)},
                    {"max_noise", std::stoi(maxNoiseStr)},
                    {"num_exp", std::stoi(numExpStr)},
                    {"target", el[15].isEmpty() ? "dry" : el[15].toStdString()}
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
                int numGroups = (numMethods + 4) / 5;
                for (int groupIdx = 0; groupIdx < numGroups; ++groupIdx) {
                    std::ostringstream s;
                    s << std::string(PATH_TO_PICTURES) << "out_" << modelName.toStdString() << "_group" << groupIdx << ".png";
                    QString path = QString::fromStdString(s.str());

                    if (QFile::exists(path)) {
                        QWidget *graphContainer = new QWidget();
                        graphContainer->setFixedWidth(static_cast<int>(Defaults::IMAGES_WIDTH) + 30);
                        QVBoxLayout *containerLayout = new QVBoxLayout(graphContainer);
                        QLabel *titleLabel = new QLabel(modelName + " (Group " + QString::number(groupIdx + 1) + ")");
                        titleLabel->setAlignment(Qt::AlignCenter);
                        titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
                        containerLayout->addWidget(titleLabel);
                        QWidget *imageContainer = new QWidget();
                        imageContainer->setFixedSize(static_cast<int>(Defaults::IMAGES_WIDTH) + 10, static_cast<int>(Defaults::IMAGES_HEIGHT) + 10);
                        QVBoxLayout *imageLayout = new QVBoxLayout(imageContainer);
                        imageLayout->setContentsMargins(0, 0, 0, 0);

                        QLabel *imageLabel = new QLabel();
                        imageLabel->setAlignment(Qt::AlignCenter);
                        QPixmap pixmap(path);

                        imageLabel->setPixmap(pixmap.scaled(static_cast<int>(Defaults::IMAGES_WIDTH), static_cast<int>(Defaults::IMAGES_HEIGHT), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                        imageLabel->setMinimumSize(static_cast<int>(Defaults::IMAGES_WIDTH), static_cast<int>(Defaults::IMAGES_HEIGHT));

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