#ifndef GD_HPP
#define GD_HPP

#include "../../COMMON/Matrix.hpp"
#include <functional>

namespace CP {
    namespace ML {
        namespace GD {
            using GradientFunction = std::function<Common::Matrix(const Common::Matrix&, const Common::Matrix&, const Common::Matrix&)>;

            enum StopCriterion {
                N_ITERS=1,
                W_DIFF=2,
                // GRAD_DIFF=4,
                // ...=2^n
            };

            class GradientDescent {
            public:
                GradientDescent(size_t batchSize, double learningRate, size_t stopCriterion, const std::vector<double>& criterions, GradientFunction func);
                void Run(const Common::Matrix& X, const Common::Matrix& y);
                Common::Matrix Weights() const;

            protected:
                size_t _curIter;
                size_t _batchSize;
                size_t _stopCriterion;
                size_t _nIters;
                double _wTolerance;
                double _learningRate;
                double _epsilon;
                Common::Matrix _w;
                GradientFunction _calcGrad;

                void GradientStep(const Common::Matrix& X, const Common::Matrix& y);
                void InitializeParams(size_t nParams);
            };
        }
    }
}

#endif // GD_HPP