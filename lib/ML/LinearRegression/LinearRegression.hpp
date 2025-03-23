#ifndef LINEARREGRESSION_HPP
#define LINEARREGRESSION_HPP

#include "../GradientDescent/Adam.hpp"
#include "../GradientDescent/GD.hpp"

class LinearRegression {
public:
    LinearRegression();
    void Fit(const Ndarray& X, const Ndarray& y);
    Ndarray Predict(const Ndarray& X) const;
private:
    Adam _GDrunner;
};

#endif // LINEARREGRESSION_HPP