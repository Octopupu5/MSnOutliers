#ifndef CP_2025_LINE_H
#define CP_2025_LINE_H

#include "Drawable.hpp"
#include <string>

class Line : public Drawable {
public:
    Line(double k, double b, const std::string& color = "green", double thickness = 1.0, double transparency = 1.0)
    : _k(k), _b(b), _color(color), _thickness(thickness), _transparency(transparency) {}

    virtual json toJson() const override {
        json j;
        j["type"] = "line";
        j["k"] = _k;
        j["b"] = _b;
        j["color"] = _color;
        j["thickness"] = _thickness;
        j["transparency"] = _transparency;
        return j;
    }

private:
    double _k;
    double _b;
    std::string _color;
    double _thickness;
    double _transparency;
};

#endif  // CP_2025_LINE_H