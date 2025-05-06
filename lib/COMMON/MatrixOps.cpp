#include "Matrix.hpp"

namespace CP {
    namespace Common {
        static Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] + rhs[i][j];
                }
            }
            return result;
        }
        
        static Matrix operator-(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] - rhs[i][j];
                }
            }
            return result;
        }
        
        static Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            if (lhs_cols != rhs_rows) {
                throw std::invalid_argument("Lhs_cols != rhs_rows");
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
        
        static Matrix operator/(const Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    Feature value = rhs[i][j];
                    if (std::fabs(value.Value()) <= 0.000001) {
                        value = 0.000001 - 2 * 0.000001 * (value < 0.0);
                    }
                    result[i][j] = lhs[i][j] / value;
                }
            }
            return result;
        }
        
        static Matrix operator+(const Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] + scalar;
                }
            }
            return result;
        }
        
        static Matrix operator-(const Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] - scalar;
                }
            }
            return result;
        }
        
        static Matrix operator*(const Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] * scalar;
                }
            }
            return result;
        }
        
        static Matrix operator/(const Matrix& lhs, double scalar) {
            if (std::fabs(scalar) <= 0.000001) {
                scalar = 0.000001 - 2 * 0.000001 * (scalar < 0.0);
            }
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            Matrix result(lhs_rows, Row(lhs_cols));
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    result[i][j] = lhs[i][j] / scalar;
                }
            }
            return result;
        }

        static Matrix operator+(double scalar, const Matrix& rhs) {
            return rhs + scalar;
        }
        
        static Matrix operator-(double scalar, const Matrix& rhs) {
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            Matrix result(rhs_rows, Row(rhs_cols));
            for (size_t i = 0; i < rhs_rows; ++i) {
                for (size_t j = 0; j < rhs_cols; ++j) {
                    result[i][j] = scalar - rhs[i][j].Value();
                }
            }
            return result;
        }
        
        static Matrix operator*(double scalar, const Matrix& rhs) {
            return rhs * scalar;
        }
        
        static Matrix operator/(double scalar, const Matrix& rhs) {
            return rhs / scalar;
        }
        
        static Matrix& operator+=(Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] += rhs[i][j];
                }
            }
            return lhs;
        }
        
        static Matrix& operator-=(Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] -= rhs[i][j];
                }
            }
            return lhs;
        }
        
        static Matrix& operator*=(Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] *= rhs[i][j];
                }
            }
            return lhs;
        }
        
        static Matrix& operator/=(Matrix& lhs, const Matrix& rhs) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            auto [rhs_rows, rhs_cols] = Shape(rhs);
            EnsureSameShape(lhs, rhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    Feature value = rhs[i][j];
                    if (std::fabs(value.Value()) <= 0.000001) {
                        value = 0.000001 - 2 * 0.000001 * (value < 0.0);
                    }
                    lhs[i][j] /= value;
                }
            }
            return lhs;
        }
        
        static Matrix& operator+=(Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] += scalar;
                }
            }
            return lhs;
        }
        
        static Matrix& operator-=(Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] -= scalar;
                }
            }
            return lhs;
        }
        
        static Matrix& operator*=(Matrix& lhs, double scalar) {
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] *= scalar;
                }
            }
            return lhs;
        }
        
        static Matrix& operator/=(Matrix& lhs, double scalar) {
            if (std::fabs(scalar) <= 0.000001) {
                scalar = 0.000001 - 2 * 0.000001 * (scalar < 0.0);
            }
            auto [lhs_rows, lhs_cols] = Shape(lhs);
            for (size_t i = 0; i < lhs_rows; ++i) {
                for (size_t j = 0; j < lhs_cols; ++j) {
                    lhs[i][j] /= scalar;
                }
            }
            return lhs;
        }

        static Matrix operator+(Matrix&& lhs, const Matrix& rhs) {
            lhs += rhs;
            return std::move(lhs);
        }
        
        static Matrix operator+(const Matrix& lhs, Matrix&& rhs) {
            rhs += lhs;
            return std::move(rhs);
        }
        
        static Matrix operator+(Matrix&& lhs, Matrix&& rhs) {
            lhs += rhs;
            return std::move(lhs);
        }
        
        static Matrix operator-(Matrix&& lhs, const Matrix& rhs) {
            lhs -= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator-(const Matrix& lhs, Matrix&& rhs) {
            rhs -= lhs;
            rhs *= -1.0;
            return std::move(rhs);
        }
        
        static Matrix operator-(Matrix&& lhs, Matrix&& rhs) {
            lhs -= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator*(Matrix&& lhs, const Matrix& rhs) {
            lhs *= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator*(const Matrix& lhs, Matrix&& rhs) {
            rhs *= lhs;
            return std::move(rhs);
        }
        
        static Matrix operator*(Matrix&& lhs, Matrix&& rhs) {
            lhs *= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator/(Matrix&& lhs, const Matrix& rhs) {
            lhs /= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator/(const Matrix& lhs, Matrix&& rhs) {
            Matrix result = lhs;
            result /= rhs;
            return result;
        }
        
        static Matrix operator/(Matrix&& lhs, Matrix&& rhs) {
            lhs /= rhs;
            return std::move(lhs);
        }
        
        static Matrix operator+(Matrix&& lhs, double scalar) {
            lhs += scalar;
            return std::move(lhs);
        }
        
        static Matrix operator+(double scalar, Matrix&& rhs) {
            rhs += scalar;
            return std::move(rhs);
        }
        
        static Matrix operator-(Matrix&& lhs, double scalar) {
            lhs -= scalar;
            return std::move(lhs);
        }
        
        static Matrix operator-(double scalar, Matrix&& rhs) {
            rhs -= scalar;
            rhs *= -1.0;
            return std::move(rhs);
        }
        
        static Matrix operator*(Matrix&& lhs, double scalar) {
            lhs *= scalar;
            return std::move(lhs);
        }
        
        static Matrix operator*(double scalar, Matrix&& rhs) {
            rhs *= scalar;
            return std::move(rhs);
        }
        
        static Matrix operator/(Matrix&& lhs, double scalar) {
            lhs /= scalar;
            return std::move(lhs);
        }        
    }
}