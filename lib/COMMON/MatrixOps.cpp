#include "Matrix.hpp"

std::vector<Feature> Matrix::operator[](int index) const {
    return _data[index];
}

std::vector<Feature>& Matrix::operator[](int index) {
    return _data[index];
}

Matrix Matrix::operator+(const Matrix& other) const {
    EnsureShapeForOperator(other);
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] + other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    EnsureShapeForOperator(other);
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] - other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    EnsureShapeForOperator(other);
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Matrix Matrix::operator/(const Matrix& other) const {
    EnsureShapeForOperator(other);
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            Feature value = other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
            if (std::fabs(value.Value()) <= 0.000001) {
                value = 0.000001 - 2 * 0.000001 * (value < 0.0);
            }
            result._data[i][j] = _data[i][j] / value;
        }
    }
    return result;
}

Matrix Matrix::operator+(double scalar) const {
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] + scalar;
        }
    }
    return result;
}

Matrix Matrix::operator-(double scalar) const {
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] - scalar;
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * scalar;
        }
    }
    return result;
}

Matrix Matrix::operator/(double scalar) const {
    if (std::fabs(scalar) <= 0.000001) {
        scalar = 0.000001 - 2 * 0.000001 * (scalar < 0.0);
    }
    Matrix result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] / scalar;
        }
    }
    return result;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] += other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] -= other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] *= other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Matrix& Matrix::operator/=(const Matrix& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            Feature value = other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
            if (std::fabs(value.Value()) <= 0.000001) {
                value = 0.000001 - 2 * 0.000001 * (value < 0.0);
            }
            _data[i][j] /= value;
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] += scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] -= scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator/=(double scalar) {
    if (std::fabs(scalar) <= 0.000001) {
        scalar = 0.000001 - 2 * 0.000001 * (scalar < 0.0);
    }
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] /= scalar;
        }
    }
    return *this;
}