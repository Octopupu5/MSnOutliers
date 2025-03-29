#include <iostream>
#include <unordered_map>
#include "COMMON/FileParser.hpp"
#include "MS/LeastSquares.hpp"
#include "COMMON/Metrics.hpp"
#include "nlohmann/json.hpp"
#include "config.h"

using json = nlohmann::json;
using stats = std::unordered_map<std::string, CP::Common::Matrix>;

stats runOnMethods(const CP::Common::RegressionData& data, const std::vector<std::string>& methods) {
    stats res;
    for (auto& el : methods) {
        if (el == "LSM") {
            res["LSM"] = CP::MS::LeastSquaresMethod(data).compute();
        } else if (el == "AVG") {
            // ... other methods;
        }
    }
    return res;
}

int main() {
    std::string path = std::string(DATA_DIR) + "/source.csv";
    uint32_t numFeatures = 3;
    CP::Common::FileParser parser;
    CP::Common::RegressionData data = parser.parseCSV(path, numFeatures);
    CP::Common::Matrix target({{0}, {1}, {1}, {1}});
    CP::Common::Metrics calc;

    stats computed = runOnMethods(data, {"LSM"});
    for (auto &[name, v] : computed) {
        std::cout << "Error on " << name << ": " << calc.meanSquaredError(target, v) << std::endl;
    }

    return 0;
}