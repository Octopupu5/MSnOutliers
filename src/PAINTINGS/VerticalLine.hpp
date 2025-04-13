#ifndef CP_2025_VERTICALLINE_H
#define CP_2025_VERTICALLINE_H

#include "Drawable.hpp"
#include <string>

class VerticalLine : public Drawable {
public:
    VerticalLine(double x, double min_y, double max_y, const std::string& color = "black", double thickness = 1.0, double transparency = 1.0)
    : _x(x), _min_y(min_y), _max_y(max_y), _color(color), _thickness(thickness), _transparency(transparency) {}

    virtual json toJson() const override {
        json j;
        j["type"] = "line";
        j["orientation"] = "vertical";
        j["x"] = _x;
        j["min_y"] = _min_y;
        j["max_y"] = _max_y;
        j["color"] = _color;
        j["thickness"] = _thickness;
        j["transparency"] = _transparency;
        return j;
    }

private:
    double _x;
    double _min_y;
    double _max_y;
    std::string _color;
    double _thickness;
    double _transparency;
};

#endif  // CP_2025_VERTICALLINE_H