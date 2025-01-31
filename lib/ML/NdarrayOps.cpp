#include "Ndarray.hpp"

std::vector<Feature> Ndarray::operator[](int index) const {
    return _data[index];
}

std::vector<Feature>& Ndarray::operator[](int index) {
    return _data[index];
}

Ndarray Ndarray::operator+(const Ndarray& other) const {
    EnsureShapeForOperator(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] + other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Ndarray Ndarray::operator-(const Ndarray& other) const {
    EnsureShapeForOperator(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] - other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Ndarray Ndarray::operator*(const Ndarray& other) const {
    EnsureShapeForOperator(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Ndarray Ndarray::operator/(const Ndarray& other) const {
    EnsureShapeForOperator(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] / other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return result;
}

Ndarray Ndarray::operator+(double scalar) const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] + scalar;
        }
    }
    return result;
}

Ndarray Ndarray::operator-(double scalar) const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] - scalar;
        }
    }
    return result;
}

Ndarray Ndarray::operator*(double scalar) const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * scalar;
        }
    }
    return result;
}

Ndarray Ndarray::operator/(double scalar) const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] / scalar;
        }
    }
    return result;
}

Ndarray& Ndarray::operator+=(const Ndarray& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] += other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Ndarray& Ndarray::operator-=(const Ndarray& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] -= other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Ndarray& Ndarray::operator*=(const Ndarray& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] *= other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Ndarray& Ndarray::operator/=(const Ndarray& other) {
    EnsureShapeForOperator(other);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] /= other._data[other._rows == 1 ? 0 : i][other._cols == 1 ? 0 : j];
        }
    }
    return *this;
}

Ndarray& Ndarray::operator+=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] += scalar;
        }
    }
    return *this;
}

Ndarray& Ndarray::operator-=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] -= scalar;
        }
    }
    return *this;
}

Ndarray& Ndarray::operator*=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] *= scalar;
        }
    }
    return *this;
}

Ndarray& Ndarray::operator/=(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j] /= scalar;
        }
    }
    return *this;
}