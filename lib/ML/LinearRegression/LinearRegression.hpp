#ifndef LINEARREGRESSION_HPP
#define LINEARREGRESSION_HPP

#include "../GradientDescent/Adam.hpp"
#include "../GradientDescent/GD.hpp"

class LinearRegression {
public:
    LinearRegression();
    void Fit(const Matrix& X, const Matrix& y);
    Matrix Predict(const Matrix& X) const;
private:
    Adam _GDrunner;
};

#endif // LINEARREGRESSION_HPP