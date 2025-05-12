#include "GradMethod.hpp"
namespace CP {
    namespace MS {
        Eigen::VectorXd GradMethod::compute() {
            auto n = _featuresMatrix.rows();
            auto d = _featuresMatrix.cols();
            Vec w  = initialWeights_;
            Vec prev_w = initialWeights_;
            int ep = 0;
            for (ep = 0; ep < _epochs; ++ep) {
                Vec grad = Vec::Zero(d);
                double loss = 0.0;
                for (int i = 0; i < n; ++i) {
                    double pred = _featuresMatrix.row(i).dot(w);
                    double rem = pred - _targetMatrix[i];
                    double g = this->gradient(rem);
                    grad += g * _featuresMatrix.row(i).transpose();
                    loss += this->loss(rem);
                }
                grad /= n;
                if (grad.norm() < 1e-1) {
                    break;
                }
                prev_w = w;
                w -= _learningRate * grad;
                if ((w - prev_w).norm() < 1e-6) {
                    std::cout << "Out on " << ep << std::endl;
                    break;
                }
            }
            return w;
        }
    } // namespace MS;
} // namespace CP;
