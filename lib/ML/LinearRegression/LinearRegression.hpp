#ifndef LINEARREGRESSION_HPP
#define LINEARREGRESSION_HPP

#include "Ndarray.hpp"
#include "GradientDescent/Adam.hpp"

class LinearRegression {
public:
    LinearRegression();
    void Fit(const Ndarray& data);
    Ndarray Predict(const Ndarray& data) const;
private:
    Adam GDrunner;
};

#endif // LINEARREGRESSION_HPP