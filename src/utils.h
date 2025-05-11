#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <future>
#include "MS/LeastSquares.hpp"
#include "MS/Huber.hpp"
#include "MS/Tukey.hpp"
#include "MS/TheilSen.hpp"
#include "MS/MinAbsDeviation.hpp"
#include "COMMON/Metrics.hpp"
#include "COMMON/DataDeNoiser.hpp"
#include "../json/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using stats = std::unordered_map<std::string, Eigen::VectorXd>;
using distType = CP::Distributions::ErrorDistributions::DistributionType;

namespace CP {
    namespace UTILS {
        const std::unordered_map<std::string, distType> dists = {
            {"\"Normal\"", distType::Normal},
            {"\"Student\"", distType::Student},
            {"\"Cauchy\"", distType::Cauchy},
            {"\"Laplace\"", distType::Laplace}
        };

        const std::unordered_set<std::string> validModels{"LSM", "HUB", "TUK", "LAD", "THS"};
        const std::unordered_set<std::string> validDists{"Normal", "Student", "Cauchy", "Laplace", "Scale"};
        const std::unordered_set<std::string> validMLModels{"None", "IForest", "DBSCAN", "KDE", "KNN"};

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

        stats runOnMethods(const json& method, size_t numNoise, CP::Common::DataDeNoiser &deNoiser, bool dumpModels) {
            stats res;
            for (auto &i : method.items()) {
                auto params = i.value();
                if (params["noise"]["type"].dump() == "\"Scale\"") {
                    CP::Distributions::ErrorDistributions dummyDist(distType::Normal);
                    deNoiser.noise(numNoise, dummyDist, true);
                } else {
                    CP::Distributions::ErrorDistributions dist(dists.at(params["noise"]["type"].dump()), params["noise"]["param1"], params["noise"]["param2"]);
                    deNoiser.noise(numNoise, dist);
                }
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
                    f << name << "\n" << vec << "\n";
                }
                f.close();
            }
            return res;
        }
        
        CP::Common::Matrix fromEigenVec(const Eigen::VectorXd& vec) {
            std::vector<std::vector<double>> f;
            auto len = vec.size();
            for (int i = 0; i < len; ++i) {
                f.push_back({vec[i]});
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

        CP::Common::Matrix parseTarget(std::string tar) {
            std::stringstream s(tar);
            std::string cur;
            CP::Common::Matrix res;
            
            while(std::getline(s, cur, ';')) {
                res.push_back({std::stod(cur)});
            }
            return res;
        }

        double calculateAverage(const std::vector<double>& values) {
            if (values.empty()) return 0.0;
            return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        }
    } // namespace UTILS
} // namespace CP

#endif // UTILS_H