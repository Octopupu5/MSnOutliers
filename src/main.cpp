#include <iostream>
#include <unordered_map>
#include <fstream>
#include "COMMON/FileParser.hpp"
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

using json = nlohmann::json;
using stats = std::unordered_map<std::string, Eigen::VectorXd>;

stats runOnMethods(const CP::Common::RegressionData& data, const json& method, size_t numNoise) {
    stats res;
    CP::Distributions::ErrorDistributions dist(CP::Distributions::ErrorDistributions::DistributionType::Normal);
    for (auto &i : method.items()) {
        auto params = i.value();
        if (i.key() == "LSM") {
            auto model = CP::MS::LeastSquaresMethod(data);
            model.makeNoise(numNoise, dist);
            res["LSM"] = model.compute();
        } else if (i.key() == "HUB") {
            auto model = CP::MS::Huber(data, params["delta"], params["eps"], params["lr"]);
            model.makeNoise(numNoise, dist);
            res["HUB"] = model.compute();
        } else if (i.key() == "TUK") {
            auto model = CP::MS::Huber(data, params["delta"], params["eps"], params["lr"]);
            model.makeNoise(numNoise, dist);
            res["TUK"] = model.compute();
        } else if (i.key() == "THS") {
            auto model = CP::MS::TheilSen(data);
            model.makeNoise(numNoise, dist);
            res["THS"] = model.compute();
        } else if (i.key() == "LAD") {
            auto model = CP::MS::MinAbsDeviation(data, params["eps"], params["lr"]);
            model.makeNoise(numNoise, dist);
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


int main(int argc, char **argv) {
    std::string path = std::string(DATA_DIR) + "/source.csv";
    uint32_t numFeatures = 3;
    CP::Common::FileParser parser;
    CP::Common::RegressionData data = parser.parseCSV(path, numFeatures);
    CP::Common::Matrix target({{-1.2}, {2.7}, {3.5}, {4.78}}); // -1.2 + 2.7x1 + 3.5x2 + 4.78x3
    CP::Common::Metrics calc;

    json methods;
    assert(argc == 2 && "No path to file!");
    auto path_to_models = std::string(argv[1]);
    std::ifstream j(path_to_models);
    j >> methods;
    j.close();

    for (auto &method : methods) {
        std::vector<std::pair<double, double>> errors;
        for (size_t numNoise = 0; numNoise <= 50; ++numNoise) {
            double avg_error = 0;
            size_t numExperiments = 10;
            for (size_t numExperiment = 0; numExperiment < numExperiments; ++numExperiment) {
                stats computed = runOnMethods(data, method, numNoise);
                auto [name, weights] = *(computed.begin());
                avg_error += calc.meanSquaredError(target, std::move(fromEigenVec(weights)));
            }
            avg_error /= numExperiments;
            errors.push_back({numNoise, avg_error});
        }
        std::string conf = std::string(SRC_CONFIGS_DIR) + "/config_" + method.items().begin().key() + ".json";
        std::string out = std::string(SRC_OUTPUTS_DIR) + "/out_" + method.items().begin().key() + ".png";
        Graph graph("Error vs Noise on " + method.items().begin().key() + " method", "Noise Level", "Error", conf, out);
        auto plot = std::make_shared<Scatter>(errors, "blue", 15, 0.9);
        graph.addObject(plot);
        graph.saveConfig();
        graph.draw();
    }
    return 0;
}