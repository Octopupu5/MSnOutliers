#include "Metrics.hpp"

namespace CP {
    namespace Common {
        double Metrics::meanAbsoluteError(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            return pow(L1Norm(trueValues, predictions), 2) / rows / cols;
        }

        double Metrics::meanSquaredError(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            return pow(L2Norm(trueValues, predictions), 2) / rows / cols;
        }

        double Metrics::rootMeanSquaredError(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            return L2Norm(trueValues, predictions) / rows / cols;
        }

        double Metrics::r2Score(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            double meanTrue = ColumnWiseMean(trueValues)[0][0].Value();
            double totalSumOfSquares = L2Norm(trueValues, Matrix(rows, Row(cols, meanTrue)));
            double residualSumOfSquares = L2Norm(trueValues, predictions);
            return 1.0 - (residualSumOfSquares / totalSumOfSquares);
        }

        // double Metrics::meanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions) {
        //     auto [rows, cols] = Shape(trueValues);
        //     return ((trueValues - predictions).Abs() / trueValues).Sum() / rows / cols * 100;
        // }

        // double Metrics::symmetricMeanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions) {
        //     auto [rows, cols] = Shape(trueValues);
        //     return ((trueValues - predictions).Abs() / ((Abs(trueValues) + Abs(predictions)) / 2.0)).Sum() / rows / cols * 100;
        // }

        double Metrics::adjustedR2Score(const Matrix& trueValues, const Matrix& predictions, int numFeatures) {
            double r2 = r2Score(trueValues, predictions);
            auto [n, cols] = Shape(trueValues);
            if (n <= numFeatures + 1) {
                throw std::invalid_argument("Number of features must be less than the number of samples.");
            }
            return 1.0 - ((1.0 - r2) * (n - 1) / (n - numFeatures - 1));
        }
    }
}