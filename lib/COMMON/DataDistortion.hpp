#ifndef DATADISTORTION_HPP
#define DATADISTORTION_HPP

#include "Matrix.hpp"
#include "../DISTRIBUTIONS/ErrorDistributions.hpp"
#include <random>

namespace CP {
    namespace Common {
        class DataDistortion {
        public:
            DataDistortion(Distributions::ErrorDistributions::DistributionType distributionType, double param1 = 0.0, double param2 = 1.0);
            Matrix distortMatrix(const Matrix& X, std::mt19937& gen);
            Matrix multiplyFeatures(const Matrix& X, std::mt19937& gen);
            Matrix addConstantNoise(const Matrix& X, double value);
            Matrix dropoutMatrix(const Matrix& X, std::mt19937& gen, double dropoutRate);
        private:
            Distributions::ErrorDistributions _distribution;
        };
    }
}

#endif // DATADISTORTION_HPP