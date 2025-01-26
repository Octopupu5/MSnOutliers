#include "Metrics.hpp"

void Metrics::validateInputs(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    if (trueValues.size() != predictions.size()) {
        throw std::invalid_argument("Vectors trueValues and predictions must have the same size.");
    }
    if (trueValues.empty()) {
        throw std::invalid_argument("Input vectors must not be empty.");
    }
}

double Metrics::meanAbsoluteError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);
    double sum = 0.0;
    for (size_t i = 0; i < trueValues.size(); ++i) {
        sum += std::abs(trueValues[i] - predictions[i]);
    }
    return sum / trueValues.size();
}

double Metrics::meanSquaredError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);
    double sum = 0.0;
    for (size_t i = 0; i < trueValues.size(); ++i) {
        double diff = trueValues[i] - predictions[i];
        sum += diff * diff;
    }
    return sum / trueValues.size();
}

double Metrics::rootMeanSquaredError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    return std::sqrt(meanSquaredError(trueValues, predictions));
}

double Metrics::r2Score(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);

    double meanTrue = std::accumulate(trueValues.begin(), trueValues.end(), 0.0) / trueValues.size();

    double totalSumOfSquares = 0.0;
    double residualSumOfSquares = 0.0;

    for (size_t i = 0; i < trueValues.size(); ++i) {
        double diffTrue = trueValues[i] - meanTrue;
        double diffPred = trueValues[i] - predictions[i];

        totalSumOfSquares += diffTrue * diffTrue;
        residualSumOfSquares += diffPred * diffPred;
    }

    return 1.0 - (residualSumOfSquares / totalSumOfSquares);
}

double Metrics::meanAbsolutePercentageError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);
    double sum = 0.0;
    // very dangerous to use on zeros
    for (size_t i = 0; i < trueValues.size(); ++i) {
        if (trueValues[i] == 0) continue;
        sum += std::abs((trueValues[i] - predictions[i]) / trueValues[i]);
    }
    return (sum / trueValues.size()) * 100;
}

double Metrics::symmetricMeanAbsolutePercentageError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);
    double sum = 0.0;
    for (size_t i = 0; i < trueValues.size(); ++i) {
        double denominator = (std::abs(trueValues[i]) + std::abs(predictions[i])) / 2.0;
        if (denominator == 0) continue;
        sum += std::abs(trueValues[i] - predictions[i]) / denominator;
    }
    return (sum / trueValues.size()) * 100;
}

double Metrics::medianAbsoluteError(const std::vector<double>& trueValues, const std::vector<double>& predictions) {
    validateInputs(trueValues, predictions);
    std::vector<double> errors(trueValues.size());
    for (size_t i = 0; i < trueValues.size(); ++i) {
        errors[i] = std::abs(trueValues[i] - predictions[i]);
    }
    std::nth_element(errors.begin(), errors.begin() + errors.size() / 2, errors.end());
    return errors[errors.size() / 2];
}

double Metrics::adjustedR2Score(const std::vector<double>& trueValues, const std::vector<double>& predictions, int numFeatures) {
    validateInputs(trueValues, predictions);
    double r2 = r2Score(trueValues, predictions);
    size_t n = trueValues.size();
    if (n <= numFeatures + 1) {
        throw std::invalid_argument("Number of features must be less than the number of samples.");
    }
    return 1.0 - ((1.0 - r2) * (n - 1) / (n - numFeatures - 1));
}