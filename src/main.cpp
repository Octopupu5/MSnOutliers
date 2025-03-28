#include <iostream>
#include <unordered_map>
#include "COMMON/FileParser.hpp"
#include "MS/LeastSquares.hpp"
#include "COMMON/Metrics.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using stats = std::unordered_map<std::string, Matrix>;

stats runOnMethods(const CP::RegressionData& data, const std::vector<std::string>& methods) {
    stats res;
    for (auto& el : methods) {
        if (el == "LSM") {
            res["LSM"] = CP::LeastSquaresMethod(data).compute();
        } else if (el == "AVG") {
            // ... other methods;
        }
    }
    return res;
}

int main() {
    std::string path = "/Users/heritagetime/VSCode/MSnOutliers/src/data/source.csv";
    uint32_t numFeatures = 3;
    CP::FileParser parser;
    CP::RegressionData data = parser.parseCSV(path, numFeatures);
    Matrix target({{0}, {1}, {1}, {1}});
    Metrics calc;

    stats computed = runOnMethods(data, {"LSM"});
    for (auto &[name, v] : computed) {
        std::cout << "Error on " << name << ": " << calc.meanSquaredError(target, v) << std::endl;
    }

    return 0;
}