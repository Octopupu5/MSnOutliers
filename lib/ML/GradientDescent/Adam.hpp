#ifndef ADAM_HPP
#define ADAM_HPP

#include "GD.hpp"

namespace CP {
    namespace ML {
        namespace GD {
            class Adam : public GradientDescent {
            public:
                Adam(size_t batch_size, double learning_rate, size_t stop_criterion, const std::vector<double>& criterions, GradientFunction func, double beta1 = 0.9, double beta2 = 0.999) 
                : GradientDescent(batch_size, learning_rate, stop_criterion, criterions, func), _beta1(beta1), _beta2(beta2) {}
            private:
                Common::Matrix _m;
                Common::Matrix _v;
                double _beta1;
                double _beta2;

                void GradientStep(const Common::Matrix& X, const Common::Matrix& y) {
                    Common::Matrix g = _calcGrad(X, y, _w);
                    _m = _m * _beta1 + g * (1 - _beta1);
                    _v = _v * _beta2 + g * g * (1 - _beta2);
                    _w -= _m * _learningRate / (1 - std::pow(_beta1, _curIter)) / (_v / (1 - std::pow(_beta2, _curIter)) + 0.000001).Sqrt();
                }

                void InitializeParams(size_t nParams) {
                    _w = Common::Matrix(nParams, 1);
                    _m = Common::Matrix(nParams, 1);
                    _v = Common::Matrix(nParams, 1);
                }
            };
        }
    }
}

#endif // ADAM_HPP