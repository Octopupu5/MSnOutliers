#include "OneClassSVM.hpp"

namespace CP {
    namespace ML {
        // OneClassSVM::OneClassSVM(float nu, float gamma) : nu(nu), gamma(gamma), rho(0.0) {}
        OneClassSVM::OneClassSVM(float gamma) : _gamma(gamma), _rho(0.0) {}

        void OneClassSVM::Fit(const Common::Matrix& data) {
            auto [rows, cols] = Shape(data);
            _kernelMatrix = Common::Matrix(rows, Common::Row(rows));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    _kernelMatrix[i][j] = Kernel(data[i], data[j]);
                }
            }
            _supportVectors = data;
            _coefficients = Common::Matrix(rows, Common::Row(1, 1.0 / rows));
            DecisionBoundary(data);
        }

        Common::Matrix OneClassSVM::Predict(const Common::Matrix& data) const {
            auto [nSamples, nFeatures] = Shape(data);
            auto [rows, cols] = Shape(_supportVectors);
            Common::Matrix predictions(nSamples, Common::Row(1));
            for (size_t i = 0; i < nSamples; ++i) {
                double decision = 0.0;
                for (size_t j = 0; j < rows; ++j) {
                    decision += _coefficients[j][0].Value() * Kernel(data[i], _supportVectors[j]);
                }
                decision -= _rho;
                predictions[i][0] = (decision >= 0) ? 0 : 1;
            }
            return predictions;
        }

        double OneClassSVM::Kernel(const Common::Row& x1, const Common::Row& x2) const {
            return std::exp(-_gamma * std::pow(L2Norm(x1, x2), 2));
        }

        void OneClassSVM::DecisionBoundary(const Common::Matrix& data) {
            auto [rows, cols] = Shape(_supportVectors);
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    sum += _coefficients[j][0].Value() * _kernelMatrix[i][j].Value();
                }
            }
            _rho = sum / rows;
        }
    }
}