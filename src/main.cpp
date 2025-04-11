#include <iostream>
#include <unordered_map>
#include <fstream>
#include "COMMON/FileParser.hpp"
#include "MS/LeastSquares.hpp"
#include "MS/Huber.hpp"
#include "MS/Tukey.hpp"
#include "COMMON/Metrics.hpp"
#include "../json/single_include/nlohmann/json.hpp"
#include "config.h"

using json = nlohmann::json;
using stats = std::unordered_map<std::string, Eigen::VectorXd>;

stats runOnMethods(const CP::Common::RegressionData& data, const std::vector<std::string>& methods) {
    stats res;
    std::ofstream weights;
    weights.open("models.txt");
    CP::Distributions::ErrorDistributions dist(CP::Distributions::ErrorDistributions::DistributionType::StudentT);
    for (auto& el : methods) {
        if (el == "LSM") {
            auto model = CP::MS::LeastSquaresMethod(data);
            model.makeNoise(25, dist);
            res["LSM"] = model.compute();
        } else if (el == "HUB") {
            // need to parametrize here
            auto model = CP::MS::Huber(data, 1.345, 1000, 0.01);
            model.makeNoise(25, dist);
            res["HUB"] = model.compute();
            weights << "HUB: " << res["HUB"] << std::endl;
        }
        else if (el == "TUK") {
            // need to parametrize here
            auto model = CP::MS::Tukey(data, 4.685, 1000, 0.001);
            model.makeNoise(25, dist);
            res["TUK"] = model.compute();
            weights << "TUK: " << res["TUK"] << std::endl;
        }
    }
    weights.close();
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


int main() {
    std::string path = std::string(DATA_DIR) + "/source.csv";
    uint32_t numFeatures = 3;
    CP::Common::FileParser parser;
    CP::Common::RegressionData data = parser.parseCSV(path, numFeatures);
    CP::Common::Matrix target({{-1.2}, {2.7}, {3.5}, {4.78}}); // -1.2 + 2.7x1 + 3.5x2 + 4.78x3
    CP::Common::Metrics calc;

    stats computed = runOnMethods(data, {"LSM", "HUB", "TUK"});
    for (auto &[name, v] : computed) {
        std::cout << "Error on " << name << ": " << calc.meanSquaredError(target, std::move(fromEigenVec(v))) << std::endl;
    }

    return 0;
}