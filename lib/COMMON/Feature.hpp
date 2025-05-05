#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <iostream>
#include <string>
#include <cmath>

namespace CP {
    namespace Common {
        class Feature {
        public:
            Feature();
            Feature(double value);
            Feature(const std::string& name, double value);

            Feature(const Feature& other) noexcept = default;
            Feature& operator=(const Feature& other) noexcept = default;
            Feature(Feature &&rhs) noexcept = default;
            Feature& operator=(Feature &&rhs) noexcept = default;

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
        };
    }
}

#endif // FEATURE_HPP