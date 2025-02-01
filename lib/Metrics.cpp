#include "Metrics.hpp"

double Metrics::meanAbsoluteError(const Ndarray& trueValues, const Ndarray& predictions) {
    return pow(trueValues.L1Norm(predictions), 2) / trueValues.Cols() / trueValues.Rows();
}

double Metrics::meanSquaredError(const Ndarray& trueValues, const Ndarray& predictions) {
    return pow(trueValues.L2Norm(predictions), 2) / trueValues.Cols() / trueValues.Rows();
}

double Metrics::rootMeanSquaredError(const Ndarray& trueValues, const Ndarray& predictions) {
    return trueValues.L2Norm(predictions) / trueValues.Cols() / trueValues.Rows();
}

double Metrics::r2Score(const Ndarray& trueValues, const Ndarray& predictions) {
    double meanTrue = trueValues.ColumnWiseMean().At(0, 0).Value();
    double totalSumOfSquares = trueValues.L2Norm(Ndarray(trueValues.Rows(), trueValues.Cols(), meanTrue));
    double residualSumOfSquares = trueValues.L2Norm(predictions);
    return 1.0 - (residualSumOfSquares / totalSumOfSquares);
}

double Metrics::meanAbsolutePercentageError(const Ndarray& trueValues, const Ndarray& predictions) {
    return ((trueValues - predictions).Abs() / trueValues).Sum() / trueValues.Rows() / trueValues.Cols() * 100;
}

double Metrics::symmetricMeanAbsolutePercentageError(const Ndarray& trueValues, const Ndarray& predictions) {
    return ((trueValues - predictions).Abs() / ((trueValues.Abs() + predictions.Abs()) / 2.0)).Sum() / trueValues.Rows() / trueValues.Cols() * 100;
}

double Metrics::adjustedR2Score(const Ndarray& trueValues, const Ndarray& predictions, int numFeatures) {
    double r2 = r2Score(trueValues, predictions);
    size_t n = trueValues.Rows();
    if (n <= numFeatures + 1) {
        throw std::invalid_argument("Number of features must be less than the number of samples.");
    }
    return 1.0 - ((1.0 - r2) * (n - 1) / (n - numFeatures - 1));
}