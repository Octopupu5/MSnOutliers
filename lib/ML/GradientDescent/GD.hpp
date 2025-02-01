#ifndef GD_HPP
#define GD_HPP

#include "Ndarray.hpp"
#include <functional>

using GradientFunction = std::function<Ndarray(const Ndarray&, const Ndarray&, const Ndarray&)>;

enum StopCriterion {
    N_ITERS=1,
    W_DIFF=2,
    // GRAD_DIFF=4,
    // ...=2^n
};

class GradientDescent {
public:
    GradientDescent(size_t batchSize, double learningRate, size_t stopCriterion, const std::vector<double>& criterions, GradientFunction func);
    void Run(const Ndarray& X, const Ndarray& y);
    Ndarray Weights() const;

protected:
    size_t _curIter;
    size_t _batchSize;
    size_t _stopCriterion;
    size_t _nIters;
    double _wTolerance;
    double _learningRate;
    double _epsilon;
    Ndarray _w;
    GradientFunction _calcGrad;

    void GradientStep(const Ndarray& X, const Ndarray& y);
    void InitializeParams(size_t nParams);
};


#endif // GD_HPP