#include "Tukey.hpp"

namespace CP {
    namespace MS {
        double Tukey::gradient(double rem) {
            double u = rem / delta_;
            if (std::abs(u) < 1.0) {
                return (1.0 - u * u) * (1.0 - u * u);
            }
            return 0.0;
        }

        Vec Tukey::compute() {
            auto n = _featuresMatrix.rows();
            auto p = _featuresMatrix.cols();
            Vec beta = Vec::Zero(p);
            Vec weights = Vec::Ones(n);

            for (int epoch = 0; epoch < epochs_; ++epoch) {
                Vec sqrt_w = weights.array().sqrt();
                Mat Xw = _featuresMatrix;
                Vec yw = _targetMatrix;

                for (int i = 0; i < n; ++i) {
                    Xw.row(i) *= sqrt_w(i);
                        yw(i) *= sqrt_w(i);
                }

                Vec beta_new = (Xw.transpose() * Xw).ldlt().solve(Xw.transpose() * yw);
                if ((beta - beta_new).norm() < 1e-6) {
                    break;
                }

                beta = beta_new;
                Vec residuals = _targetMatrix - _featuresMatrix * beta;
                for (int i = 0; i < n; ++i) {
                    weights(i) = gradient(residuals(i));
                }
            }
            return beta;
        }
    } // namespace MS;
} // namespace CP;


