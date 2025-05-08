#ifndef Matrix_HPP
#define Matrix_HPP

#include "Feature.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>

namespace CP {
    namespace Common {
        using Row = std::vector<Feature>;
        using Matrix = std::vector<Row>;
        static std::pair<size_t, size_t> Shape(const Matrix &data) {
            size_t rows = data.size();
            size_t cols = rows ? data[0].size() : 0;
            return {rows, cols};
        }

        static void CheckIndices(const Matrix& data, size_t row, size_t col) {
            auto [rows, cols] = Shape(data);
            if (row >= rows || col >= cols || row < 0 || col < 0) {
                throw std::out_of_range("Index out of bounds");
            }
        }

        static void EnsureSameShape(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            if (lhs_rows != rhs_rows || lhs_cols != rhs_cols) {
                throw std::invalid_argument("Arrays must have the same shape for this operation");
            }
        }

        static Matrix T(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            Matrix result(cols, Row(cols));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[j][i] = data[i][j];
                }
            }
            return result;
        }

        static void AddColumns(Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < rhs_cols; ++j) {
                    lhs[i].push_back(rhs[i][j]);
                }
            }
        }

        static Matrix Inv(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            if (rows != cols) {
                throw std::invalid_argument("Matrix must be square for inversion");
            }
            
            Matrix augmented(rows, Row(cols * 2));
            
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    augmented[i][j] = data[i][j];
                }
                augmented[i][i + cols] = 1.0;
            }
            
            for (size_t i = 0; i < rows; i++) {
                size_t max_row = i;
                for (size_t k = i + 1; k < rows; k++) {
                    if (std::abs(augmented[k][i].Value()) > std::abs(augmented[max_row][i].Value())) {
                        max_row = k;
                    }
                }
                
                if (std::abs(augmented[max_row][i].Value()) < 1e-10) {
                    throw std::runtime_error("Matrix is singular and cannot be inverted");
                }
                
                if (max_row != i) {
                    for (size_t j = 0; j < 2 * cols; j++) {
                        std::swap(augmented[i][j], augmented[max_row][j]);
                    }
                }
                
                double pivot = augmented[i][i].Value();
                for (size_t j = 0; j < 2 * cols; j++) {
                    augmented[i][j] = augmented[i][j].Value() / pivot;
                }
                
                for (size_t k = 0; k < rows; k++) {
                    if (k != i) {
                        double factor = augmented[k][i].Value();
                        for (size_t j = 0; j < 2 * cols; j++) {
                            augmented[k][j] = augmented[k][j].Value() - factor * augmented[i][j].Value();
                        }
                    }
                }
            }
            
            Matrix inverse(rows, Row(cols));
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    inverse[i][j] = augmented[i][j + cols];
                }
            }
            
            return inverse;
        }

        static Matrix Matmul(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            if (lhs_cols != rhs_rows) {
                throw std::invalid_argument("lhs_cols != rhs_rows matrix multiplication");
            }
            Matrix result(lhs_rows, Row(rhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < rhs_cols; ++j) {
                    for (size_t k = 0; k < lhs_cols; ++k) {
                        result[i][j] += lhs[i][k] * rhs[k][j];
                    }
                }
            }
            return result;
        }
        
        static Matrix ColumnWiseMean(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            Matrix result(1, Row(cols));
            for (size_t j = 0; j < cols; ++j) {
                double accumulated = 0.0;
                for (size_t i = 0; i < rows; ++i) {
                    accumulated += data[i][j].Value();
                }
                result[0][j] = accumulated / rows;
            }
            return result;
        }
        
        static Matrix RowWiseMean(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            Matrix result(rows, Row(1));
            for (size_t i = 0; i < rows; ++i) {
                double accumulated = 0.0;
                for (size_t j = 0; j < cols; ++j) {
                    accumulated += data[i][j].Value();
                }
                result[i][0] = accumulated / cols;
            }
            return result;
        }
        
        // 1 2 3
        // 4 5 6
        // 1 2 5
        // --->>
        // 6 9 14
        static Matrix ColumnWiseSum(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            Matrix result(1, Row(cols));
            for (size_t j = 0; j < cols; ++j) {
                for (size_t i = 0; i < rows; ++i) {
                    result[0][j] += data[i][j];
                }
            }
            return result;
        }
        
        static Matrix RowWiseSum(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            Matrix result(rows, Row(1));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][0] += data[i][j];
                }
            }
            return result;
        }
        
        static double Sum(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            double result = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result += data[i][j].Value();
                }
            }
            return result;
        }
        
        template <typename Function>
        static typename std::enable_if<std::is_invocable_r<Feature, Function, Feature>::value, Matrix>::type Apply(const Matrix& data, Function&& f) {
            auto [rows, cols] = Shape(data);
            Matrix result(rows, Row(cols));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] = f(data[i][j]);
                }
            }
            return result;
        }
        
        template <typename Function>
        static typename std::enable_if<std::is_invocable_r<Feature, Function, double>::value, Matrix>::type Apply(const Matrix& data, Function&& f) {
            auto [rows, cols] = Shape(data);
            Matrix result(rows, Row(cols));
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i][j] = f(data[i][j].Value());
                }
            }
            return result;
        }
        
        static Matrix Sqrt(const Matrix& data) {
            return Apply(data, [](double x) { return std::sqrt(x); });
        }
        
        static Matrix Abs(const Matrix& data) {
            return Apply(data, [](double x) { return std::fabs(x); });
        }
        
        static Matrix Pow(const Matrix& data, double power) {
            return Apply(data, [power](double x) { return std::pow(x, power); });
        }
        
        static double L1Norm(const Matrix& lhs, const Matrix& rhs) {
            EnsureSameShape(lhs, rhs);
            double result = 0;
            auto [rows, cols] = Shape(lhs);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result += std::fabs((lhs[i][j] - rhs[i][j]).Value());
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
                    double diff = (lhs[i][j] - rhs[i][j]).Value();
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
                double diff = (lhs[i] - rhs[i]).Value();
                result += diff * diff;
            }
            return std::sqrt(result);
        }
        
        static void Print(const Matrix& data) {
            auto [rows, cols] = Shape(data);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    std::cout << std::setw(10) << data[i][j].Value() << " ";
                }
                std::cout << "\n";
            }
        }
    }
}

#endif // Matrix_HPP