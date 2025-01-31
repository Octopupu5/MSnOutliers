#ifndef ADAGRAD_HPP
#define ADAGRAD_HPP

#include "GD.hpp"

class AdaGrad : GradientDescent {
public:
    AdaGrad(size_t batch_size, double learning_rate, size_t stop_criterion, const std::vector<double>& criterions, GradientFunction func, double gamma) 
    : GradientDescent(batch_size, learning_rate, stop_criterion, criterions, func) {}
    
    void GradientStep(const Ndarray& X, const Ndarray& y) {
        Ndarray g = _calcGrad(X, y, _w);
        _G = _G + g * g;
        _w -= g * _learningRate / (_G + 0.000001).Sqrt();
    }

    void InitializeParams(size_t nParams) {
        _w = Ndarray(nParams, 1);
        _G = Ndarray(nParams, 1);
    }
private:
    Ndarray _G;
};

#endif // ADAGRAD_HPP