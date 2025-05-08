#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "MS/LeastSquares.hpp"
#include "MS/Huber.hpp"
#include "MS/Tukey.hpp"
#include "MS/TheilSen.hpp"
#include "MS/MinAbsDeviation.hpp"
#include "COMMON/Metrics.hpp"
#include "../json/single_include/nlohmann/json.hpp"
#include "config.h"
#include "PAINTINGS/Graph.hpp"
#include "PAINTINGS/Scatter.hpp"
#include "COMMON/DataDeNoiser.hpp"

#include <future>

using json = nlohmann::json;
using stats = std::unordered_map<std::string, Eigen::VectorXd>;
using distType = CP::Distributions::ErrorDistributions::DistributionType;

namespace {
    const std::unordered_map<std::string, distType> dists = {
        {"\"Normal\"", distType::Normal},
        {"\"StudentT\"", distType::StudentT},
        {"\"Cauchy\"", distType::Cauchy},
        {"\"Laplace\"", distType::Laplace}
    };

    const std::unordered_set<std::string> validModels{"LSM", "HUB", "TUK", "LAD", "THS"};
    const std::unordered_set<std::string> validDists{"Normal", "StudentT", "Cauchy", "Laplace"};
    const std::unordered_set<std::string> validMLModels{"None", "IForest", "DBSCAN", "KDE", "KNN"};

    stats runOnMethods(const json& method, size_t numNoise, CP::Common::DataDeNoiser &deNoiser) {
        stats res;
        CP::Distributions::ErrorDistributions dist(CP::Distributions::ErrorDistributions::DistributionType::Normal);
        for (auto &i : method.items()) {
            auto params = i.value();
            CP::Distributions::ErrorDistributions dist(dists.at(params["noise"]["type"].dump()), params["noise"]["param1"], params["noise"]["param2"]);
            deNoiser.noise(numNoise, dist);
            const CP::Common::RData processedData = deNoiser.denoise(params["mlmodel"]);
            if (i.key() == "LSM") {
                auto model = CP::MS::LeastSquaresMethod(processedData);
                res["LSM"] = model.compute();
            } else if (i.key() == "HUB") {
                auto model = CP::MS::Huber(processedData, params["delta"], params["eps"], params["lr"]);
                res["HUB"] = model.compute();
            } else if (i.key() == "TUK") {
                auto model = CP::MS::Huber(processedData, params["delta"], params["eps"], params["lr"]);
                res["TUK"] = model.compute();
            } else if (i.key() == "THS") {
                auto model = CP::MS::TheilSen(processedData);
                res["THS"] = model.compute();
            } else if (i.key() == "LAD") {
                auto model = CP::MS::MinAbsDeviation(processedData, params["eps"], params["lr"]);
                res["LAD"] = model.compute();
            }
        }
        return res;
    }
    
    CP::Common::Matrix fromEigenVec(const Eigen::VectorXd& vec) {
        std::vector<std::vector<CP::Common::Feature>> f;
        auto len = vec.size();
        for (int i = 0; i < len; ++i) {
            f.push_back({CP::Common::Feature(vec[i])});
        }
        return CP::Common::Matrix(f);
    }

    bool validateJson(const json& j) {
        if (!j.contains("models") || !j["models"].is_array()) return false;

        for (const auto& item : j["models"]) {
            auto checkIter = item.items().begin();
            checkIter.operator++();
            if (checkIter != item.items().end()) return false;

            auto name = item.items().begin().key();
            if (validModels.find(name) == validModels.end()) return false;

            if (!item[name].contains("delta") || !item[name]["delta"].is_number_float()) return false;
            if (!item[name].contains("eps") || !item[name]["eps"].is_number_integer()) return false;
            if (!item[name].contains("lr") || !item[name]["lr"].is_number_float()) return false;
            if (!item[name].contains("noise")) return false;
            if (!item[name].contains("mlmodel")) return false;
            if (!item[name]["mlmodel"].is_string() || validMLModels.find(item[name]["mlmodel"]) == validMLModels.end()) return false;

            if (!item[name]["noise"].contains("param1") || !item[name]["noise"]["param1"].is_number_float()) return false;
            if (!item[name]["noise"].contains("param2") || !item[name]["noise"]["param2"].is_number_float()) return false;
            if (!item[name]["noise"].contains("type") || !item[name]["noise"]["type"].is_string() || validDists.find(item[name]["noise"]["type"]) == validDists.end()) return false;
        
            if (!item[name].contains("path") || !item[name]["path"].is_string()) return false;
            if (!item[name].contains("num_feat") || !item[name]["num_feat"].is_number_integer()) return false;
        }
        return true;
    }
}

int main(int argc, char **argv) {
    CP::Common::FileParser parser;
    CP::Common::Matrix target({{-1.2}, {2.7}, {3.5}, {4.78}}); // -1.2 + 2.7x1 + 3.5x2 + 4.78x3 -> parametrize
    CP::Common::Metrics calc;
    json methods;
    assert(argc == 2 && "No path to file!");
    auto path_to_models = std::string(argv[1]);
    std::ifstream j(path_to_models);
    j >> methods;
    j.close();

    if (!validateJson(methods)) {
        std::cout << "Malformed json-data!" << std::endl;
        return 1;
    }
    
    for (auto &method : methods["models"]) {
        std::string path = method.items().begin().value()["path"];
        if (path.empty()) {
            path = std::string(DATA_DIR) + "/source.csv";
        }
        
        CP::Common::RegressionData data = parser.parseCSV(path, method.items().begin().value()["num_feat"]);

        // std::vector<std::pair<double, double>> errors;
        // for (size_t numNoise = 0; numNoise <= 50; ++numNoise) {
        //     double avg_error = 0;
        //     size_t numExperiments = 10;
        //     for (size_t numExperiment = 0; numExperiment < numExperiments; ++numExperiment) {
        //         stats computed = runOnMethods(method, numNoise, deNoiser);
        //         auto [name, weights] = *(computed.begin());
        //         avg_error += calc.meanSquaredError(target, std::move(fromEigenVec(weights)));
        //     }
        //     avg_error /= numExperiments;
        //     errors.push_back({numNoise, avg_error});
        // }

        struct ExperimentResult {
            size_t noise;
            double error;
        };

        size_t maxNoise = 50;
        size_t numExperiments = 10;

        unsigned n_threads = std::thread::hardware_concurrency();
        std::vector<std::future<ExperimentResult>> futures;
        std::vector<std::vector<double>> errors_by_noise(maxNoise + 1);

        for (size_t numNoise = 0; numNoise <= maxNoise; ++numNoise) {
            for (size_t numExperiment = 0; numExperiment < numExperiments; ++numExperiment) {
                while (futures.size() >= n_threads) {
                    for (auto it = futures.begin(); it != futures.end(); ) {
                        if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                            ExperimentResult res = it->get();
                            errors_by_noise[res.noise].push_back(res.error);
                            it = futures.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
                futures.push_back(std::async(std::launch::async, [&, numNoise]() {
                    CP::Common::DataDeNoiser deNoiser(data);
                    stats computed = runOnMethods(method, numNoise, deNoiser);
                    auto [name, weights] = *(computed.begin());
                    double error = calc.meanSquaredError(target, fromEigenVec(weights));
                    return ExperimentResult{numNoise, error};
                }));
            }
        }

        for (auto& fut : futures) {
            ExperimentResult res = fut.get();
            errors_by_noise[res.noise].push_back(res.error);
        }

        std::vector<std::pair<double, double>> errors;
        for (size_t n = 0; n <= maxNoise; ++n) {
            double avg = 0.0;
            if(!errors_by_noise[n].empty()) {
                avg = std::accumulate(errors_by_noise[n].begin(), errors_by_noise[n].end(), 0.0) / errors_by_noise[n].size();
            }
            errors.emplace_back(n, avg);
        }

        std::string conf = std::string(SRC_CONFIGS_DIR) + "/config_" + method.items().begin().key() + "_" + std::string(method.items().begin().value()["mlmodel"]) + ".json";
        std::string out = std::string(SRC_OUTPUTS_DIR) + "/out_" + method.items().begin().key() + "_" + std::string(method.items().begin().value()["mlmodel"]) + ".png";
        Graph graph("Error vs Noise on " + method.items().begin().key() + " method with " + std::string(method.items().begin().value()["mlmodel"]) + " ML algorightm", "Noise Level", "Error", conf, out);
        auto plot = std::make_shared<Scatter>(errors, "blue", 15, 0.9);
        graph.addObject(plot);
        graph.saveConfig();
        graph.draw();
    }
    return 0;
}