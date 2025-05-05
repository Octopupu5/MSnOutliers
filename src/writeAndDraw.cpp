#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include "../json/single_include/nlohmann/json.hpp"
#include "config.h"
#include "COMMON/Matrix.hpp"
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