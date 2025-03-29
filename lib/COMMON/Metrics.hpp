#ifndef METRICS_HPP
#define METRICS_HPP

#include "Matrix.hpp"

#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <algorithm>

namespace CP {
    namespace Common {
        class Metrics {
        public:
            static double meanAbsoluteError(const Matrix& trueValues, const Matrix& predictions);
            static double meanSquaredError(const Matrix& trueValues, const Matrix& predictions);
            static double rootMeanSquaredError(const Matrix& trueValues, const Matrix& predictions);
            static double r2Score(const Matrix& trueValues, const Matrix& predictions);
            static double meanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions);
            static double symmetricMeanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions);
            static double adjustedR2Score(const Matrix& trueValues, const Matrix& predictions, int numFeatures);
        };
    }
}

#endif // METRICS_HPP
