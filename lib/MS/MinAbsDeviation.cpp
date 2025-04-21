#include "MinAbsDeviation.hpp"

namespace CP {
    namespace MS {
        double smooth_sign(double x) {
            return x / (std::abs(x) + 1e-6);
        }

        Vec MinAbsDeviation::compute() {
            auto n = _featuresMatrix.rows();
            auto d = _featuresMatrix.cols();
            Vec beta = Vec::Zero(d);

            for (int epoch = 0; epoch < _epochs; ++epoch) {
                Vec resid = _targetMatrix - _featuresMatrix * beta;
                Vec grad = Vec::Zero(d);
                for (int i = 0; i < n; ++i) {
                    grad -= smooth_sign(resid(i)) * _featuresMatrix.row(i).transpose();
                }

                beta -= _learningRate * grad / n;
            }

            return beta;
        }
    } // namespace MS;
} // namespace CP;