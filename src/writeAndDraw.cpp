#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include "../json/single_include/nlohmann/json.hpp"
#include "config.h"
#include "COMMON/Matrix.hpp"
#include "COMMON/DataDistortion.hpp"
#include "DISTRIBUTIONS/ErrorDistributions.hpp"

#include "PAINTINGS/Graph.hpp"
#include "PAINTINGS/Line.hpp"
#include "PAINTINGS/Scatter.hpp"
#include "PAINTINGS/FunctionPlot.hpp"
#include "PAINTINGS/VerticalLine.hpp"
#include "PAINTINGS/HorizontalLine.hpp"

using json = nlohmann::json;
using namespace CP::Common;

int main() {
    std::string conf = std::string(SRC_CONFIGS_DIR) + "/config.json";
    std::string out = std::string(SRC_OUTPUTS_DIR) + "/out.png";
    Graph graph("aboba", "x axis", "y axis", conf, out);

    double true_slope = 2.0, true_intercept = 1.0;
    std::vector<double> x_vals, y_vals;
    size_t n = 50;
    for (size_t i = 0; i < n; ++i) {
        double x = static_cast<double>(i);
        x_vals.push_back(x);
        y_vals.push_back(true_slope * x + true_intercept);
    }

//     std::vector<std::vector<Feature>> data(n, std::vector<Feature>(2));
//     std::mt19937 gen(std::random_device{}());

//     for (size_t i = 0; i < n; ++i) {
//         double x_value = static_cast<double>(i);
//         double y_value = true_slope * x_value + true_intercept;
//         data[i][0] = Feature(x_value);
//         data[i][1] = Feature(y_value);
//     }

//     Matrix dataMatrix(data);

//     CP::Distributions::ErrorDistributions::DistributionType distType = CP::Distributions::ErrorDistributions::Normal;
//     DataDistortion distortion(distType, 0.0, 2.5);
//     Matrix noisyData = distortion.distortMatrix(dataMatrix, gen);

//     Matrix x = noisyData.ColumnMatrix(0);
//     Matrix y = noisyData.ColumnMatrix(1);

//     double sum_x = x.Sum();
//     double sum_y = y.Sum();
//     double sum_xy = (x * y).Sum();
//     double sum_xx = (x * x).Sum();

//     double b = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
//     double a = (sum_y - b * sum_x) / n;

    auto line = std::make_shared<Line>(true_slope, true_intercept, "green", 2.0, 0.8);
    std::vector<std::pair<double, double>> points;
    std::mt19937 gen(std::random_device{}());
    std::normal_distribution<> noise(0, 2.5);
    for (size_t i = 0; i < n; ++i) {
        double x = x_vals[i];
        double y = y_vals[i] + noise(gen);
        points.push_back({x, y});
    }
    auto scatter = std::make_shared<Scatter>(points, "red", 10.0, 0.9);
    auto vertLine = std::make_shared<VerticalLine>(3, -10, 10);
    auto horLine = std::make_shared<HorizontalLine>(3, -10, 10);
    auto func = std::make_shared<FunctionPlot>([&] (const double &a) {
        return sin(a) * cos(a);
    }, 0, 40, 1000, "blue", 1, 0.7);

    graph.addObject(scatter);
    graph.addObject(line);
    graph.addObject(vertLine);
    graph.addObject(horLine);
    graph.addObject(func);
    
    graph.saveConfig();
    graph.draw();

    return 0;
}