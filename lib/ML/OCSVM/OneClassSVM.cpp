#include "OneClassSVM.hpp"

namespace CP {
    namespace ML {
        // OneClassSVM::OneClassSVM(float nu, float gamma) : nu(nu), gamma(gamma), rho(0.0) {}
        OneClassSVM::OneClassSVM(float gamma) : _gamma(gamma), _rho(0.0) {}

        void OneClassSVM::Fit(const Common::Matrix& data) {
            size_t rows = data.Rows();
            Common::Matrix kernelMatrix(rows, rows);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    kernelMatrix.At(i, j) = Kernel(data.RowMatrix(i), data.RowMatrix(j));
                }
            }
            Common::Matrix alphas(rows, 1);
            alphas.Fill(1.0 / rows);
            _supportVectors = data;
            _coefficients = alphas;
            DecisionBoundary(data);
        }

        Common::Matrix OneClassSVM::Predict(const Common::Matrix& data) const {
            size_t nSamples = data.Rows();
            Common::Matrix predictions(nSamples, 1);
            for (size_t i = 0; i < nSamples; ++i) {
                double decision = 0.0;
                for (size_t j = 0; j < _supportVectors.Rows(); ++j) {
                    decision += _coefficients.At(j, 0).Value() * Kernel(data.RowMatrix(i), _supportVectors.RowMatrix(j));
                }
                decision -= _rho;
                predictions.At(i, 0) = (decision >= 0) ? 0 : 1;
            }
            return predictions;
        }

        double OneClassSVM::Kernel(const Common::Matrix& x1, const Common::Matrix& x2) const {
            return std::exp(-_gamma * std::pow(x1.L2Norm(x2), 2));
        }

        void OneClassSVM::DecisionBoundary(const Common::Matrix& data) {
            double sum = 0.0;
            for (size_t i = 0; i < _supportVectors.Rows(); ++i) {
                for (size_t j = 0; j < _supportVectors.Rows(); ++j) {
                    sum += _coefficients.At(j, 0).Value() * Kernel(_supportVectors.RowMatrix(i), _supportVectors.RowMatrix(j));
                }
            }
            _rho = sum / _supportVectors.Rows();
        }
    }
}