#ifndef CP_2025_HORIZONTALLINE_H
#define CP_2025_HORIZONTALLINE_H

#include "Drawable.hpp"
#include <string>

class HorizontalLine : public Drawable {
public:
    HorizontalLine(double y, double min_x, double max_x, const std::string& color = "black", double thickness = 1.0, double transparency = 1.0)
    : _y(y), _min_x(min_x), _max_x(max_x), _color(color), _thickness(thickness), _transparency(transparency) {}

    virtual json toJson() const override {
        json j;
        j["type"] = "line";
        j["orientation"] = "horizontal";
        j["y"] = _y;
        j["min_x"] = _min_x;
        j["max_x"] = _max_x;
        j["color"] = _color;
        j["thickness"] = _thickness;
        j["transparency"] = _transparency;
        return j;
    }

private:
    double _y;
    double _min_x;
    double _max_x;
    std::string _color;
    double _thickness;
    double _transparency;
};

#endif  // CP_2025_HORIZONTALLINE_H