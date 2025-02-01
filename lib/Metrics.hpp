#ifndef METRICS_HPP
#define METRICS_HPP

#include "Ndarray.hpp"

#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <algorithm>

class Metrics {
public:
    static double meanAbsoluteError(const Ndarray& trueValues, const Ndarray& predictions);
    static double meanSquaredError(const Ndarray& trueValues, const Ndarray& predictions);
    static double rootMeanSquaredError(const Ndarray& trueValues, const Ndarray& predictions);
    static double r2Score(const Ndarray& trueValues, const Ndarray& predictions);
    static double meanAbsolutePercentageError(const Ndarray& trueValues, const Ndarray& predictions);
    static double symmetricMeanAbsolutePercentageError(const Ndarray& trueValues, const Ndarray& predictions);
    static double adjustedR2Score(const Ndarray& trueValues, const Ndarray& predictions, int numFeatures);
};

#endif // METRICS_HPP
