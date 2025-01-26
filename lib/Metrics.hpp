#ifndef METRICS_HPP
#define METRICS_HPP

#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <algorithm>

class Metrics {
public:
    static double meanAbsoluteError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double meanSquaredError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double rootMeanSquaredError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double r2Score(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double meanAbsolutePercentageError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double symmetricMeanAbsolutePercentageError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double medianAbsoluteError(const std::vector<double>& trueValues, const std::vector<double>& predictions);

    static double adjustedR2Score(const std::vector<double>& trueValues, const std::vector<double>& predictions, int numFeatures);
private:
    static void validateInputs(const std::vector<double>& trueValues, const std::vector<double>& predictions);
};

#endif // METRICS_HPP
