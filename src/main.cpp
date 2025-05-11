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
        {"\"Student\"", distType::Student},
        {"\"Cauchy\"", distType::Cauchy},
        {"\"Laplace\"", distType::Laplace}
    };

    const std::unordered_set<std::string> validModels{"LSM", "HUB", "TUK", "LAD", "THS"};
    const std::unordered_set<std::string> validDists{"Normal", "Student", "Cauchy", "Laplace"};
    const std::unordered_set<std::string> validMLModels{"None", "IForest", "DBSCAN", "KDE", "KNN"};

    stats runOnMethods(const json& method, size_t numNoise, CP::Common::DataDeNoiser &deNoiser, bool dumpModels) {
        stats res;
        CP::Distributions::ErrorDistributions dist(CP::Distributions::ErrorDistributions::DistributionType::Normal);
        for (auto &i : method.items()) {
            auto params = i.value();
            CP::Distributions::ErrorDistributions dist(dists.at(params["noise"]["type"].dump()), params["noise"]["param1"], params["noise"]["param2"]);
            deNoiser.noise(numNoise, dist);
            const CP::Common::RData processedData = deNoiser.denoise(params["mlmodel"]["type"], params["mlmodel"]["param1"], params["mlmodel"]["param2"]);
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
        if (dumpModels) {
            std::ofstream f("report.txt");
            for (auto &[name, vec] : res) {
                f << name << " " << vec << "\n";
            }
            f.close();
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
            if (!item[name]["mlmodel"].contains("param1") || !item[name]["mlmodel"]["param1"].is_number_float()) return false;
            if (!item[name]["mlmodel"].contains("param2") || !item[name]["mlmodel"]["param2"].is_number_float()) return false;
            if (!item[name]["mlmodel"].contains("type") || validMLModels.find(item[name]["mlmodel"]["type"]) == validMLModels.end()) return false;

            if (!item[name]["noise"].contains("param1") || !item[name]["noise"]["param1"].is_number_float()) return false;
            if (!item[name]["noise"].contains("param2") || !item[name]["noise"]["param2"].is_number_float()) return false;
            if (!item[name]["noise"].contains("type") || !item[name]["noise"]["type"].is_string() || validDists.find(item[name]["noise"]["type"]) == validDists.end()) return false;
        
            if (!item[name].contains("path") || !item[name]["path"].is_string()) return false;
            if (!item[name].contains("num_feat") || !item[name]["num_feat"].is_number_integer()) return false;
            if (!item[name].contains("min_noise") || !item[name]["min_noise"].is_number_integer()) return false;
            if (!item[name].contains("max_noise") || !item[name]["max_noise"].is_number_integer()) return false;
            if (!item[name].contains("num_exp") || !item[name]["num_exp"].is_number_integer()) return false;
            if (!item[name].contains("target") || !item[name]["target"].is_string()) return false;
        }
        return true;
    }

    struct RegressionMetrics {
        double MAE;
        double MSE;
        double RMSE;
        double MAPE;
        double SMAPE;
    };
    
    struct DetectionMetrics {
        double precision;
        double recall;
        double f1_score;
    };
    
    struct NoiseParams {
        double param1;
        double param2;
    };
    
    struct MLModelParams {
        double param1;
        double param2;
    };
    
    struct MetricsByNoiseLevel {
        int noise_level;
        RegressionMetrics metrics;
        DetectionMetrics detection_metrics;
    };
    
    struct ResultMetrics {
        std::string model;
        std::string mlmodel;
        std::string noise_type;
        NoiseParams noise_params;
        MLModelParams mlmodel_params;
        std::string data_path;
        int num_features;
        int num_experiments;
        int min_noise;
        int max_noise;
        std::vector<MetricsByNoiseLevel> metrics_by_noise_level;
    };

    void saveMetricsToJson(const std::vector<ResultMetrics>& allResults, const std::string& filepath) {
        json j;
        json resultArray = json::array();
    
        for (const auto& result : allResults) {
            json resultJson;
            resultJson["model"] = result.model;
            resultJson["mlmodel"] = result.mlmodel;
            resultJson["noise_type"] = result.noise_type;
            resultJson["noise_params"] = {
                {"param1", result.noise_params.param1},
                {"param2", result.noise_params.param2}
            };
            resultJson["mlmodel_params"] = {
                {"param1", result.mlmodel_params.param1},
                {"param2", result.mlmodel_params.param2}
            };
            resultJson["data_path"] = result.data_path;
            resultJson["num_features"] = result.num_features;
            resultJson["num_experiments"] = result.num_experiments;
            resultJson["min_noise"] = result.min_noise;
            resultJson["max_noise"] = result.max_noise;
    
            json metricsArray = json::array();
            for (const auto& metricByNoise : result.metrics_by_noise_level) {
                json metricJson;
                metricJson["noise_level"] = metricByNoise.noise_level;
                metricJson["metrics"] = {
                    {"MAE", metricByNoise.metrics.MAE},
                    {"MSE", metricByNoise.metrics.MSE},
                    {"RMSE", metricByNoise.metrics.RMSE},
                    {"MAPE", metricByNoise.metrics.MAPE},
                    {"SMAPE", metricByNoise.metrics.SMAPE}
                };
                metricJson["detection_metrics"] = {
                    {"precision", metricByNoise.detection_metrics.precision},
                    {"recall", metricByNoise.detection_metrics.recall},
                    {"f1_score", metricByNoise.detection_metrics.f1_score}
                };
                metricsArray.push_back(metricJson);
            }
            resultJson["metrics_by_noise_level"] = metricsArray;
            resultArray.push_back(resultJson);
        }
    
        j["results"] = resultArray;
        
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << j.dump(4);
            file.close();
        } else {
            std::cerr << "Error opening file for writing: " << filepath << std::endl;
        }
    }
}

CP::Common::Matrix parseTarget(std::string tar) {
    std::stringstream s(tar);
    std::string cur;
    CP::Common::Matrix res;
    
    while(std::getline(s, cur, ';')) {
        res.push_back({std::stod(cur)});
    }
    return res;
}

int main(int argc, char **argv) {
    CP::Common::FileParser parser;
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

    std::vector<std::string> regressionMetrics({"mse", "mae", "rmse", "mape", "smape"});
    std::vector<std::string> detectionMetrics({"precision", "recall", "f1_score"});
    struct ExperimentResult {
        size_t noise;
        double mse;
        double mae;
        double rmse;
        double mape;
        double smape;
        double precision;
        double recall;
        double f1;
    };

    std::unordered_map<std::string, std::vector<json>> methodGroups;
    for (auto &method : methods["models"]) {
        std::string methodName = method.items().begin().key();
        methodGroups[methodName].push_back(method);
    }
    std::vector<ResultMetrics> allResults;
    for (auto &[methodName, methodGroup] : methodGroups) {
        size_t graphCount = (methodGroup.size() + 4) / 5;    
        for (size_t graphIdx = 0; graphIdx < graphCount; ++graphIdx) {
            size_t startIdx = graphIdx * 5;
            size_t endIdx = std::min(startIdx + 5, methodGroup.size());
            std::string graphTitle = "Error vs Noise on " + methodName + " method (Group " + std::to_string(graphIdx + 1) + ")";
            std::string configPath = std::string(SRC_CONFIGS_DIR) + "/config_" + methodName + "_group" + std::to_string(graphIdx) + ".json";
            std::string outputPath = std::string(SRC_OUTPUTS_DIR) + "/out_" + methodName + "_group" + std::to_string(graphIdx) + ".png";
            Graph graph(graphTitle, "Noise Level", "Error", configPath, outputPath);
            const std::array<std::string, 5> colors = {"blue", "red", "green", "purple", "cyan"};
            for (size_t i = startIdx; i < endIdx; ++i) {
                json &method = methodGroup[i];
                std::string path = method.items().begin().value()["path"];
                if (path.empty()) {
                    path = std::string(DATA_DIR) + "/source.csv";
                }
                size_t numFeat = method.items().begin().value()["num_feat"];
                CP::Common::RegressionData data = parser.parseCSV(path, numFeat);
                std::string tar = method.items().begin().value()["target"].dump();
                bool dryRun = (tar == "\"dry\"");
                auto target = dryRun ? std::move(CP::Common::Matrix()) : std::move(parseTarget(std::move(std::string(tar.begin() + 1, tar.end() - 1))));

                if (target.size() > numFeat + 1) {
                    target.resize(numFeat + 1);
                }

                while (!dryRun && target.size() < numFeat + 1) {
                    target.push_back({0});
                }
                
                size_t minNoise = dryRun ? 0 : (static_cast<size_t>(method.items().begin().value()["min_noise"]));
                size_t maxNoise =  dryRun ? 0 : (static_cast<size_t>(method.items().begin().value()["max_noise"]));
                size_t numExperiments = dryRun ? 1 : (static_cast<size_t>(method.items().begin().value()["num_exp"]));
                
                ResultMetrics resultMetric;
                resultMetric.model = methodName;
                resultMetric.mlmodel = method.items().begin().value()["mlmodel"]["type"];
                resultMetric.noise_type = method.items().begin().value()["noise"]["type"];
                resultMetric.noise_params.param1 = method.items().begin().value()["noise"]["param1"];
                resultMetric.noise_params.param2 = method.items().begin().value()["noise"]["param2"];
                resultMetric.mlmodel_params.param1 = method.items().begin().value()["mlmodel"]["param1"];
                resultMetric.mlmodel_params.param2 = method.items().begin().value()["mlmodel"]["param2"];
                resultMetric.data_path = path;
                resultMetric.num_features = numFeat;
                resultMetric.num_experiments = numExperiments;
                resultMetric.min_noise = minNoise;
                resultMetric.max_noise = maxNoise;
                
                unsigned n_threads = std::thread::hardware_concurrency();
                std::vector<std::future<ExperimentResult>> futures;
                std::vector<std::unordered_map<std::string, std::vector<double>>> regr_errors_by_noise(maxNoise - minNoise + 1);
                std::vector<std::unordered_map<std::string, std::vector<double>>> detection_errors_by_noise(maxNoise - minNoise + 1);

                for (size_t numNoise = minNoise; numNoise <= maxNoise; ++numNoise) {
                    for (size_t numExperiment = 0; numExperiment < numExperiments; ++numExperiment) {
                        while (futures.size() >= n_threads) {
                            for (auto it = futures.begin(); it != futures.end(); ) {
                                if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                                    ExperimentResult res = it->get();
                                    regr_errors_by_noise[res.noise - minNoise]["mse"].push_back(res.mse);
                                    regr_errors_by_noise[res.noise - minNoise]["mae"].push_back(res.mae);
                                    regr_errors_by_noise[res.noise - minNoise]["rmse"].push_back(res.rmse);
                                    regr_errors_by_noise[res.noise - minNoise]["mape"].push_back(res.mape);
                                    regr_errors_by_noise[res.noise - minNoise]["smape"].push_back(res.smape);
                                    detection_errors_by_noise[res.noise - minNoise]["precision"].push_back(res.precision);
                                    detection_errors_by_noise[res.noise - minNoise]["recall"].push_back(res.recall);
                                    detection_errors_by_noise[res.noise - minNoise]["f1_score"].push_back(res.f1);
                                    it = futures.erase(it);
                                } else {
                                    ++it;
                                }
                            }
                        }
                        futures.push_back(std::async(std::launch::async, [&, numNoise, dryRun]() {
                            CP::Common::DataDeNoiser deNoiser(data);
                            stats computed = runOnMethods(method, numNoise, deNoiser, dryRun);
                            auto [name, weights] = *(computed.begin());
                            double mse = dryRun ? 0 : calc.meanSquaredError(target, fromEigenVec(weights));
                            double mae = dryRun ? 0 : calc.meanAbsoluteError(target, fromEigenVec(weights));
                            double rmse = dryRun ? 0 : calc.rootMeanSquaredError(target, fromEigenVec(weights));
                            double mape = dryRun ? 0 : calc.meanAbsolutePercentageError(target, fromEigenVec(weights));
                            double smape = dryRun ? 0 : calc.symmetricMeanAbsolutePercentageError(target, fromEigenVec(weights));
                            double precision = dryRun ? 0 : deNoiser.Precision();
                            double recall = dryRun ? 0 : deNoiser.Recall();
                            double f1 = dryRun ? 0 : deNoiser.F1();
                            return ExperimentResult{numNoise, mse, mae, rmse, mape, smape, precision, recall, f1};
                        }));
                    }
                }

                for (auto& fut : futures) {
                    ExperimentResult res = fut.get();
                    regr_errors_by_noise[res.noise - minNoise]["mse"].push_back(res.mse);
                    regr_errors_by_noise[res.noise - minNoise]["mae"].push_back(res.mae);
                    regr_errors_by_noise[res.noise - minNoise]["rmse"].push_back(res.rmse);
                    regr_errors_by_noise[res.noise - minNoise]["mape"].push_back(res.mape);
                    regr_errors_by_noise[res.noise - minNoise]["smape"].push_back(res.smape);
                    detection_errors_by_noise[res.noise - minNoise]["precision"].push_back(res.precision);
                    detection_errors_by_noise[res.noise - minNoise]["recall"].push_back(res.recall);
                    detection_errors_by_noise[res.noise - minNoise]["f1_score"].push_back(res.f1);
                }

                for (size_t n = minNoise; n <= maxNoise; ++n) {
                    size_t idx = n - minNoise;
                    double avg_mse = 0.0;
                    double avg_mae = 0.0;
                    double avg_rmse = 0.0;
                    double avg_mape = 0.0;
                    double avg_smape = 0.0;
                    double avg_precision = 0.0;
                    double avg_recall = 0.0;
                    double avg_f1 = 0.0;
                    
                    if (!regr_errors_by_noise[idx]["mse"].empty()) {
                        avg_mse = std::accumulate(regr_errors_by_noise[idx]["mse"].begin(), regr_errors_by_noise[idx]["mse"].end(), 0.0) / regr_errors_by_noise[idx]["mse"].size();
                    }
                    if (!regr_errors_by_noise[idx]["mae"].empty()) {
                        avg_mae = std::accumulate(regr_errors_by_noise[idx]["mae"].begin(), regr_errors_by_noise[idx]["mae"].end(), 0.0) / regr_errors_by_noise[idx]["mae"].size();
                    }
                    if (!regr_errors_by_noise[idx]["rmse"].empty()) {
                        avg_rmse = std::accumulate(regr_errors_by_noise[idx]["rmse"].begin(), regr_errors_by_noise[idx]["rmse"].end(), 0.0) / regr_errors_by_noise[idx]["rmse"].size();
                    }
                    if (!regr_errors_by_noise[idx]["mape"].empty()) {
                        avg_mape = std::accumulate(regr_errors_by_noise[idx]["mape"].begin(), regr_errors_by_noise[idx]["mape"].end(), 0.0) / regr_errors_by_noise[idx]["mape"].size();
                    }
                    if (!regr_errors_by_noise[idx]["smape"].empty()) {
                        avg_smape = std::accumulate(regr_errors_by_noise[idx]["smape"].begin(), regr_errors_by_noise[idx]["smape"].end(), 0.0) / regr_errors_by_noise[idx]["smape"].size();
                    }
                    
                    if (!detection_errors_by_noise[idx]["precision"].empty()) {
                        avg_precision = std::accumulate(detection_errors_by_noise[idx]["precision"].begin(), detection_errors_by_noise[idx]["precision"].end(), 0.0) / detection_errors_by_noise[idx]["precision"].size();
                    }
                    if (!detection_errors_by_noise[idx]["recall"].empty()) {
                        avg_recall = std::accumulate(detection_errors_by_noise[idx]["recall"].begin(), detection_errors_by_noise[idx]["recall"].end(), 0.0) / detection_errors_by_noise[idx]["recall"].size();
                    }
                    if (!detection_errors_by_noise[idx]["f1_score"].empty()) {
                        avg_f1 = std::accumulate(detection_errors_by_noise[idx]["f1_score"].begin(), detection_errors_by_noise[idx]["f1_score"].end(), 0.0) / detection_errors_by_noise[idx]["f1_score"].size();
                    }
                    
                    resultMetric.metrics_by_noise_level.push_back({static_cast<int>(n), {avg_mae, avg_mse, avg_rmse, avg_mape, avg_smape}, {avg_precision, avg_recall, avg_f1}});
                }
                
                allResults.push_back(resultMetric);
                std::vector<std::pair<double, double>> errors;
                for (size_t n = minNoise; n <= maxNoise; ++n) {
                    double avg = 0.0;
                    if(!regr_errors_by_noise[n - minNoise]["mse"].empty()) {
                        avg = std::accumulate(regr_errors_by_noise[n - minNoise]["mse"].begin(), regr_errors_by_noise[n - minNoise]["mse"].end(), 0.0) / regr_errors_by_noise[n - minNoise]["mse"].size();
                    }
                    errors.emplace_back(n, avg);
                }
                
                std::string mlModel = method.items().begin().value()["mlmodel"]["type"];
                std::string noiseType = method.items().begin().value()["noise"]["type"];
                std::string legendName = mlModel + " / " + noiseType;
                
                size_t colorIdx = i - startIdx;
                auto plot = std::make_shared<Scatter>(errors, colors[colorIdx], 15, 0.45, legendName);
                graph.addObject(plot);
            }                
            graph.saveConfig();
            graph.draw();
        }
    }
    saveMetricsToJson(allResults, std::string(SRC_OUTPUTS_DIR) + "/metrics.json");
    
    return 0;
}