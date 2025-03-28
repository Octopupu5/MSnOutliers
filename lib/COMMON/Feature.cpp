#include "Feature.hpp"

using Feature = CP::Common::Feature;

Feature::Feature(const std::string& name, double value) : _name(name), _value(value) {}
Feature::Feature(double value) : _name("Empty"), _value(value) {}

void Feature::SetName(const std::string& name) {
    _name = name;
}

std::string Feature::Name() const {
    return _name;
}

void Feature::SetValue(double value) {
    _value = value;
}

double Feature::Value() const {
    return _value;
}

Feature Feature::operator+(const Feature& other) const {
    return Feature(_name, _value + other._value);
}

Feature Feature::operator-(const Feature& other) const {
    return Feature(_name, _value - other._value);
}

Feature Feature::operator*(const Feature& other) const {
    return Feature(_name, _value * other._value);
}

Feature Feature::operator/(const Feature& other) const {
    if (fabs(other._value) <= 0.00000001) {
        throw std::runtime_error("Division by zero");
    }
    return Feature(_name, _value / other._value);
}

Feature& Feature::operator+=(const Feature& other) {
    _value += other._value; return *this;
}

Feature& Feature::operator-=(const Feature& other) {
    _value -= other._value; return *this;
}

Feature& Feature::operator*=(const Feature& other) {
    _value *= other._value; return *this;
}

Feature& Feature::operator/=(const Feature& other) {
    if (fabs(other._value) <= 0.00000001) {
        throw std::runtime_error("Division by zero");
    }
    _value /= other._value;
    return *this;
}

bool Feature::operator==(const Feature& other) const {
    return _name == other._name && fabs(_value - other._value) <= 0.00000001;
}

bool Feature::operator!=(const Feature& other) const {
    return !(*this == other);
}

bool Feature::operator<(const Feature& other) const {
    return _value < other._value;
}

bool Feature::operator<=(const Feature& other) const {
    return _value < other._value || this->operator==(other);
}

bool Feature::operator>(const Feature& other) const {
    return _value > other._value;
}

bool Feature::operator>=(const Feature& other) const {
    return _value > other._value || this->operator==(other);
}