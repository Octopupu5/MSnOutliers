#include "KDE.hpp"

namespace CP {
    namespace ML {
        KernelDensityEstimator::KernelDensityEstimator(float gamma) : _gamma(gamma), _rho(0.0) {}

        void KernelDensityEstimator::Fit(const Common::Matrix& data) {
            auto [rows, cols] = Shape(data);
            _trainData = data;
            _kernelMatrix = Common::Matrix(rows, Common::Row(rows));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    _kernelMatrix[i][j] = Kernel(data[i], data[j]);
                }
            }
            FindThreshold();
        }

        Common::Matrix KernelDensityEstimator::Predict(const Common::Matrix& data) const {
            auto [nSamples, nFeatures] = Shape(data);
            auto [nTrain, nCols] = Shape(_trainData);
            Common::Matrix predictions(nSamples, Common::Row(1));
            for (size_t i = 0; i < nSamples; ++i) {
                double density = 0.0;
                for (size_t j = 0; j < nTrain; ++j) {
                    density += Kernel(data[i], _trainData[j]);
                }
                density /= nTrain;
                predictions[i][0] = (density >= _rho) ? 0 : 1;
            }
            return predictions;
        }

        double KernelDensityEstimator::Kernel(const Common::Row& x1, const Common::Row& x2) const {
            return std::exp(-_gamma * L2Norm(x1, x2));
        }

        void KernelDensityEstimator::FindThreshold() {
            auto [rows, cols] = Shape(_trainData);
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                double selfDensity = 0.0;
                for (size_t j = 0; j < rows; ++j) {
                    selfDensity += _kernelMatrix[i][j].Value();
                }
                sum += (selfDensity / rows);
            }
            _rho = sum / rows;
        }
    }
}