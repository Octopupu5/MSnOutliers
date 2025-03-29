#ifndef ADAGRAD_HPP
#define ADAGRAD_HPP

#include "GD.hpp"

namespace CP {
    namespace ML {
        namespace GD {
            class AdaGrad : public GradientDescent {
            public:
                AdaGrad(size_t batch_size, double learning_rate, size_t stop_criterion, const std::vector<double>& criterions, GradientFunction func, double gamma) 
                : GradientDescent(batch_size, learning_rate, stop_criterion, criterions, func) {}
            private:
                Common::Matrix _G;
                
                void GradientStep(const Common::Matrix& X, const Common::Matrix& y) {
                    Common::Matrix g = _calcGrad(X, y, _w);
                    _G = _G + g * g;
                    _w -= g * _learningRate / (_G + 0.000001).Sqrt();
                }

                void InitializeParams(size_t nParams) {
                    _w = Common::Matrix(nParams, 1);
                    _G = Common::Matrix(nParams, 1);
                }
            };
        }
    }
}


#endif // ADAGRAD_HPP