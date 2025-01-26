#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <iostream>
#include <string>
#include <cmath>

class Feature {
public:
    Feature(double value);
    Feature(const std::string& name, double value);
    ~Feature() {}

    void SetName(const std::string& name);
    void SetValue(double value);
    
    std::string Name() const;
    double Value() const;

    Feature& operator+=(const Feature& other);
    Feature& operator-=(const Feature& other);
    Feature& operator*=(const Feature& other);
    Feature& operator/=(const Feature& other);
    Feature operator+(const Feature& other) const;
    Feature operator-(const Feature& other) const;
    Feature operator*(const Feature& other) const;
    Feature operator/(const Feature& other) const;
    
    bool operator==(const Feature& other) const;
    bool operator!=(const Feature& other) const;
    bool operator<(const Feature& other) const;
    bool operator<=(const Feature& other) const;
    bool operator>(const Feature& other) const;
    bool operator>=(const Feature& other) const;

protected:
    std::string _name;
    double _value;

private:
    Feature() {}
};

#endif // FEATURE_HPP