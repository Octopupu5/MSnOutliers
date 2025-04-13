#ifndef CP_2025_FUNCTIONPLOT_H
#define CP_2025_FUNCTIONPLOT_H

#include "Drawable.hpp"
#include <functional>
#include <vector>
#include <utility>
#include <string>

class FunctionPlot : public Drawable {
public:
    FunctionPlot(std::function<double(double)> f, double start, double end, int numSamples = 100, const std::string& color = "blue", double pointSize = 5.0, double transparency = 1.0)
    : _color(color), _pointSize(pointSize), _transparency(transparency) {
        for (int i = 0; i < numSamples; ++i) {
            _points.push_back({start + i * (end - start) / (numSamples - 1), f(start + i * (end - start) / (numSamples - 1))});
        }
    }

    virtual json toJson() const override {
        json j;
        j["type"] = "function";
        j["color"] = _color;
        j["pointSize"] = _pointSize;
        j["transparency"] = _transparency;
        j["points"] = json::array();
        for (const auto &p : _points) {
            j["points"].push_back({{"x", p.first}, {"y", p.second}});
        }
        return j;
    }
private:
    std::vector<std::pair<double, double>> _points;
    std::string _color;
    double _pointSize;
    double _transparency;
};

#endif