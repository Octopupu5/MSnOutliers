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

        double Metrics::meanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    double numerator = std::abs((trueValues[i][j] - predictions[i][j]).Value());
                    double denominator = std::abs(trueValues[i][j].Value());
                    if (denominator != 0.0)
                        sum += numerator / denominator;
                }
            }
            return sum / (rows * cols) * 100.0;
        }
        
        double Metrics::symmetricMeanAbsolutePercentageError(const Matrix& trueValues, const Matrix& predictions) {
            auto [rows, cols] = Shape(trueValues);
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    double numerator = std::abs((trueValues[i][j] - predictions[i][j]).Value());
                    double denominator = (std::abs(trueValues[i][j].Value()) + std::abs(predictions[i][j].Value())) / 2.0;
                    if (denominator != 0.0)
                        sum += numerator / denominator;
                }
            }
            return sum / (rows * cols) * 100.0;
        }
    }
}