#include "KDE.hpp"

namespace CP {
    namespace ML {
        KernelDensityEstimator::KernelDensityEstimator(float gamma) : _gamma(gamma), _rho(0.0) {}

        void KernelDensityEstimator::Fit(const Common::Matrix& data) {
            auto [rows, cols] = Common::Shape(data);
            _data = data;
            _kernelMatrix = Common::Matrix(rows, Common::Row(rows));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    _kernelMatrix[i][j] = Kernel(data[i], data[j]);
                }
            }
            FindThreshold();
        }

        Common::Matrix KernelDensityEstimator::Predict(const Common::Matrix& data) const {
            auto [nSamples, nFeatures] = Common::Shape(data);
            auto [nTrain, nCols] = Common::Shape(_data);
            Common::Matrix predictions(nSamples, Common::Row(1));
            for (size_t i = 0; i < nSamples; ++i) {
                double density = 0.0;
                for (size_t j = 0; j < nTrain; ++j) {
                    density += Kernel(data[i], _data[j]);
                }
                density /= nTrain;
                predictions[i][0] = (density >= _rho) ? 0 : 1;
            }
            return predictions;
        }

        double KernelDensityEstimator::Kernel(const Common::Row& x1, const Common::Row& x2) const {
            return std::exp(-_gamma * Common::L2Norm(x1, x2));
        }

        void KernelDensityEstimator::FindThreshold() {
            auto [rows, cols] = Common::Shape(_data);
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                double selfDensity = 0.0;
                for (size_t j = 0; j < rows; ++j) {
                    selfDensity += _kernelMatrix[i][j];
                }
                sum += (selfDensity / rows);
            }
            _rho = sum / rows;
        }
    }
}