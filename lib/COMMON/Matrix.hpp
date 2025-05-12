#ifndef Matrix_HPP
#define Matrix_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>

namespace CP {
    namespace Common {
        using Row = std::vector<double>;
        using Matrix = std::vector<Row>;
        static std::pair<size_t, size_t> Shape(const Matrix &data) {
            size_t rows = data.size();
            size_t cols = rows ? data[0].size() : 0;
            return {rows, cols};
        }

        static void EnsureSameShape(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            if (lhs_rows != rhs_rows || lhs_cols != rhs_cols) {
                throw std::invalid_argument("Arrays must have the same shape for this operation");
            }
        }
        
        template <typename Function>
        static typename std::enable_if<std::is_invocable_r<double, Function, double>::value, Matrix>::type Apply(const Matrix& data, Function&& f) {
            auto [rows, cols] = Shape(data);
            Matrix result(rows, Row(cols));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] = f(data[i][j]);
                }
            }
            return result;
        }
        
        static double L1Norm(const Matrix& lhs, const Matrix& rhs) {
            EnsureSameShape(lhs, rhs);
            double result = 0;
            auto [rows, cols] = Shape(lhs);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result += std::fabs((lhs[i][j] - rhs[i][j]));
                }
            }
            return result;
        }
        
        static double L2Norm(const Matrix& lhs, const Matrix& rhs) {
            EnsureSameShape(lhs, rhs);
            double result = 0;
            auto [rows, cols] = Shape(lhs);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    double diff = (lhs[i][j] - rhs[i][j]);
                    result += diff * diff;
                }
            }
            return std::sqrt(result);
        }

        static double L2Norm(const Row& lhs, const Row& rhs) {
            if (lhs.size() != rhs.size()) {
                throw std::invalid_argument("Arrays must have the same shape for this operation");
            }
            double result = 0;
            size_t size = lhs.size();
            for (size_t i = 0; i < size; ++i) {
                double diff = (lhs[i] - rhs[i]);
                result += diff * diff;
            }
            return std::sqrt(result);
        }
        
        static void Print(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    std::cout << std::setw(10) << data[i][j] << " ";
                }
                std::cout << "\n";
            }
        }
    }
}

#endif // Matrix_HPP