#include "DataDistortion.hpp"

namespace CP {
    namespace Common {
        DataDistortion::DataDistortion(CP::Distributions::ErrorDistributions::DistributionType distributionType, double param1, double param2)
            : _distribution(distributionType, param1, param2) {}

        Matrix DataDistortion::distortMatrix(const Matrix& X, std::mt19937& gen) {
            Matrix result(X);
            for (size_t j = 0; j < X.Cols(); ++j) {
                for (size_t i = 0; i < X.Rows(); ++i) {
                    double noise = _distribution.generate(gen);
                    result.At(i, j) += noise;
                }
            }
            return result;
        }

        Matrix DataDistortion::multiplyFeatures(const Matrix& X, std::mt19937& gen) {
            Matrix result(X);
            for (size_t i = 0; i < X.Rows(); ++i) {
                for (size_t j = 0; j < X.Cols(); ++j) {
                    double noise = _distribution.generate(gen);
                    result.At(i, j) *= (1 + noise);
                }
            }
            return result;
        }

        Matrix DataDistortion::addConstantNoise(const Matrix& X, double value) {
            Matrix result(X);
            for (size_t i = 0; i < X.Rows(); ++i) {
                for (size_t j = 0; j < X.Cols(); ++j) {
                    result.At(i, j) += value;
                }
            }
            return result;
        }

        Matrix DataDistortion::dropoutMatrix(const Matrix& X, std::mt19937& gen, double dropoutRate) {
            Matrix result(X);
            std::uniform_real_distribution<> distribution(0.0, 1.0);
            for (size_t i = 0; i < X.Rows(); ++i) {
                for (size_t j = 0; j < X.Cols(); ++j) {
                    if (distribution(gen) < dropoutRate) {
                        result.At(i, j) = 0;
                    }
                }
            }
            return result;
        }
    }
}