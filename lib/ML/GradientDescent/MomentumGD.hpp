#ifndef MOMENTUMGD_HPP
#define MOMENTUMGD_HPP

#include "GD.hpp"

class MomentumGradientDescent : public GradientDescent {
public:
    MomentumGradientDescent(size_t batch_size, double learning_rate, size_t stop_criterion, const std::vector<double>& criterions, GradientFunction func, double gamma) 
    : GradientDescent(batch_size, learning_rate, stop_criterion, criterions, func), _gamma(gamma) {}
private:
    double _gamma;
    Matrix _v;

    void GradientStep(const Matrix& X, const Matrix& y) {
        _v = _v * _gamma + _calcGrad(X, y, _w) * _learningRate;
        _w -= _v;
    }

    void InitializeParams(size_t nParams) {
        _w = Matrix(nParams, 1);
        _v = Matrix(nParams, 1);
    }
};


#endif // MOMENTUMGD_HPP