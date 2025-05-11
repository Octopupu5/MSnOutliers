#ifndef CP_2025_SCATTER_H
#define CP_2025_SCATTER_H

#include "Drawable.hpp"
#include <vector>
#include <utility>
#include <string>

class Scatter : public Drawable {
public:
    Scatter(const std::vector<std::pair<double, double>>& points, const std::string& color = "red", double pointSize = 5.0, double transparency = 1.0, const std::string& label = "")
    : _points(points), _color(color), _pointSize(pointSize), _transparency(transparency), _label(label) {}

    virtual json toJson() const override {
        json j;
        j["type"] = "points";
        j["color"] = _color;
        j["pointSize"] = _pointSize;
        j["transparency"] = _transparency;
        j["label"] = _label;
        j["points"] = json::array();
        for (const auto& [x, y] : _points) {
            j["points"].push_back({{"x", x}, {"y", y}});
        }
        return j;
    }

private:
    std::vector<std::pair<double, double>> _points;
    std::string _color;
    double _pointSize;
    double _transparency;
    std::string _label;
};

#endif  // CP_2025_SCATTER_H