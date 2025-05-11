#include <iostream>
#include <fstream>
#include "config.h"
#include "PAINTINGS/Graph.hpp"
#include "PAINTINGS/Scatter.hpp"
#include "utils.h"

int main(int argc, char **argv) {
    CP::Common::FileParser parser;
    CP::Common::Metrics calc;
    json methods;
    assert(argc == 2 && "No path to file!");
    auto path_to_models = std::string(argv[1]);
    std::ifstream j(path_to_models);
    j >> methods;
    j.close();

    if (!CP::UTILS::validateJson(methods)) {
        std::cout << "Malformed json-data!" << std::endl;
        return 1;
    }

    std::vector<std::string> regressionMetrics({"mse", "mae", "rmse", "mape", "smape"});
    std::vector<std::string> detectionMetrics({"precision", "recall", "f1_score"});

    std::unordered_map<std::string, std::vector<json>> methodGroups;
    for (auto &method : methods["models"]) {
        std::string methodName = method.items().begin().key();
        methodGroups[methodName].push_back(method);
    }
    std::vector<CP::UTILS::ResultMetrics> allResults;
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
                if (dryRun) {
                    CP::Common::DataDeNoiser deNoiser(data);
                    stats computed = CP::UTILS::runOnMethods(method, 0, deNoiser, dryRun);
                    continue;
                }
                auto target = dryRun ? CP::Common::Matrix() : CP::UTILS::parseTarget(std::string(tar.begin() + 1, tar.end() - 1));

                if (target.size() > numFeat + 1) {
                    target.resize(numFeat + 1);
                }

                while (!dryRun && target.size() < numFeat + 1) {
                    target.push_back({0});
                }
                
                size_t minNoise = dryRun ? 0 : (static_cast<size_t>(method.items().begin().value()["min_noise"]));
                size_t maxNoise =  dryRun ? 0 : (static_cast<size_t>(method.items().begin().value()["max_noise"]));
                size_t numExperiments = dryRun ? 1 : (static_cast<size_t>(method.items().begin().value()["num_exp"]));
                
                CP::UTILS::ResultMetrics resultMetric;
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
                std::vector<std::future<CP::UTILS::ExperimentResult>> futures;
                std::vector<std::unordered_map<std::string, std::vector<double>>> regr_errors_by_noise(maxNoise - minNoise + 1);
                std::vector<std::unordered_map<std::string, std::vector<double>>> detection_errors_by_noise(maxNoise - minNoise + 1);
                for (size_t numNoise = minNoise; numNoise <= maxNoise; ++numNoise) {
                    for (size_t numExperiment = 0; numExperiment < numExperiments; ++numExperiment) {
                        while (futures.size() >= n_threads) {
                            for (auto it = futures.begin(); it != futures.end(); ) {
                                if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                                    CP::UTILS::ExperimentResult res = it->get();
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
                            stats computed = CP::UTILS::runOnMethods(method, numNoise, deNoiser, dryRun);
                            auto [name, weights] = *(computed.begin());
                            double mse = dryRun ? 0 : calc.meanSquaredError(target, CP::UTILS::fromEigenVec(weights));
                            double mae = dryRun ? 0 : calc.meanAbsoluteError(target, CP::UTILS::fromEigenVec(weights));
                            double rmse = dryRun ? 0 : calc.rootMeanSquaredError(target, CP::UTILS::fromEigenVec(weights));
                            double mape = dryRun ? 0 : calc.meanAbsolutePercentageError(target, CP::UTILS::fromEigenVec(weights));
                            double smape = dryRun ? 0 : calc.symmetricMeanAbsolutePercentageError(target, CP::UTILS::fromEigenVec(weights));
                            double precision = dryRun ? 0 : deNoiser.Precision();
                            double recall = dryRun ? 0 : deNoiser.Recall();
                            double f1 = dryRun ? 0 : deNoiser.F1();
                            return CP::UTILS::ExperimentResult{numNoise, mse, mae, rmse, mape, smape, precision, recall, f1};
                        }));
                    }
                }

                for (auto& fut : futures) {
                    CP::UTILS::ExperimentResult res = fut.get();
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
                    double avg_mse = CP::UTILS::calculateAverage(regr_errors_by_noise[idx]["mse"]);
                    double avg_mae = CP::UTILS::calculateAverage(regr_errors_by_noise[idx]["mae"]);
                    double avg_rmse = CP::UTILS::calculateAverage(regr_errors_by_noise[idx]["rmse"]);
                    double avg_mape = CP::UTILS::calculateAverage(regr_errors_by_noise[idx]["mape"]);
                    double avg_smape = CP::UTILS::calculateAverage(regr_errors_by_noise[idx]["smape"]);
                    double avg_precision = CP::UTILS::calculateAverage(detection_errors_by_noise[idx]["precision"]);
                    double avg_recall = CP::UTILS::calculateAverage(detection_errors_by_noise[idx]["recall"]);
                    double avg_f1 = CP::UTILS::calculateAverage(detection_errors_by_noise[idx]["f1_score"]);
                    
                    resultMetric.metrics_by_noise_level.push_back({
                        static_cast<int>(n), 
                        {avg_mae, avg_mse, avg_rmse, avg_mape, avg_smape}, 
                        {avg_precision, avg_recall, avg_f1}
                    });
                }
                
                allResults.push_back(resultMetric);
                std::vector<std::pair<double, double>> errors;
                for (size_t n = minNoise; n <= maxNoise; ++n) {
                    double avg = CP::UTILS::calculateAverage(regr_errors_by_noise[n - minNoise]["mse"]);
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
    CP::UTILS::saveMetricsToJson(allResults, std::string(SRC_OUTPUTS_DIR) + "/metrics.json");
    
    return 0;
}