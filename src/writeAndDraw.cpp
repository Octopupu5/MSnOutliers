#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include "nlohmann/json.hpp"
#include "config.h"

using json = nlohmann::json;

int main() {
    int n;
    std::cin >> n;
    std::vector<double> x(n), y(n), xy(n), xx(n);

    for (int i = 0; i < n; ++i) {
        double cur_x, cur_y;
        std::cin >> cur_x >> cur_y;
        x[i] = cur_x;
        y[i] = cur_y;
        xy[i] = x[i] * y[i];
        xx[i] = x[i] * x[i];
    }

    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xy = std::accumulate(xy.begin(), xy.end(), 0.0);
    double sum_xx = std::accumulate(xx.begin(), xx.end(), 0.0);

    double b = (n * sum_xy - sum_x * sum_y)/(n * sum_xx - sum_x * sum_x);
    double a = (sum_y - b * sum_x)/n;

    auto objs = json::array();
    auto points = json::array();
    objs.push_back({{"type", "line"}, {"k", b}, {"b", a}, {"color", "green"}});

    for (int i = 0; i < n; ++i) {
        points.push_back({{"x", x[i]}, {"y", y[i]}});
    }

    objs.push_back({{"type", "points"}, {"points", points}, {"color", "red"}});
    json j;
    j["objects"] = objs;

    std::ofstream o(std::string(SRC_CONFIGS_DIR) + "/config.json");
    o << std::setw(4) << j << std::endl;

    std::string command = "python3 " + std::string(SRC_CONFIGS_DIR) + "/../drawer.py";
    system(command.c_str());
    return 0;
}