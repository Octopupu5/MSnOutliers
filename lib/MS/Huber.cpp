#include "Huber.hpp"
namespace CP {
    namespace MS {
        double Huber::loss(double rem) const {
            if (std::abs(rem) <= delta_) {
                return 0.5 * rem * rem;
            }
            return  delta_ * (std::abs(rem) - 0.5 * delta_);
        }

        double Huber::gradient(double rem) const {
            if (std::abs(rem) <= delta_) {
                return rem;
            }
            return delta_ * ((rem > 0) ? 1 : -1);
        }

        Eigen::VectorXd Huber::compute() {
            auto n = _featuresMatrix.rows();
            auto d = _featuresMatrix.cols();
            Vec w = Vec::Zero(d);

            for (auto ep = 0; ep < epochs_; ++ep) {
                Vec grad = Vec::Zero(d);
                double loss = 0.0;

                for (int i = 0; i < d; ++i) {
                    double pred = _featuresMatrix.row(i).dot(w);
                    double rem = pred - _targetMatrix[i];
                    double g = this->gradient(rem);
                    grad += g * _featuresMatrix.row(i).transpose();
                    loss += this->loss(rem);
                }
                grad /= n;
                w -= learningRate_ * grad;
            }
            return w;
        }
    } // namespace MS;
} // namespace CP;
