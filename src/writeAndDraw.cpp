#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include "../json/single_include/nlohmann/json.hpp"
#include "config.h"
#include "COMMON/Matrix.hpp"
#include "COMMON/DataDistortion.hpp"
#include "DISTRIBUTIONS/ErrorDistributions.hpp"

using json = nlohmann::json;
using namespace CP::Common;

int main() {
    size_t n = 50;
    double true_slope = 2.0, true_intercept = 1.0;

    std::vector<std::vector<Feature>> data(n, std::vector<Feature>(2));
    std::mt19937 gen(std::random_device{}());

    for (size_t i = 0; i < n; ++i) {
        double x_value = static_cast<double>(i);
        double y_value = true_slope * x_value + true_intercept;
        data[i][0] = Feature(x_value);
        data[i][1] = Feature(y_value);
    }

    Matrix dataMatrix(data);

    CP::Distributions::ErrorDistributions::DistributionType distType = CP::Distributions::ErrorDistributions::Normal;
    DataDistortion distortion(distType, 0.0, 2.5);
    Matrix noisyData = distortion.distortMatrix(dataMatrix, gen);

    Matrix x = noisyData.ColumnMatrix(0);
    Matrix y = noisyData.ColumnMatrix(1);

    double sum_x = x.Sum();
    double sum_y = y.Sum();
    double sum_xy = (x * y).Sum();
    double sum_xx = (x * x).Sum();

    double b = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    double a = (sum_y - b * sum_x) / n;

    auto objs = json::array();
    auto points = json::array();
    objs.push_back({{"type", "line"}, {"k", b}, {"b", a}, {"color", "green"}});

    for (size_t i = 0; i < n; ++i) {
        points.push_back({{"x", x.At(i, 0).Value()}, {"y", y.At(i, 0).Value()}});
    }

    objs.push_back({{"type", "points"}, {"points", points}, {"color", "red"}});
    json j;
    j["objects"] = objs;

    std::ofstream o(std::string(SRC_CONFIGS_DIR) + "/config.json");
    o << std::setw(4) << j << std::endl;

    std::string path = std::string(SRC_CONFIGS_DIR) + "/../drawer.py";
    std::string command = "python3 " + path;
    system(command.c_str());

    std::string img_path = std::string(SRC_CONFIGS_DIR) + "/../outputs/output.png";
    if (!std::filesystem::exists(img_path)) {
        std::cout << "No kartinka!\n";
        return 0;
    }

    return 0;
}