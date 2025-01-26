#include "Ndarray.hpp"

std::vector<Feature> Ndarray::operator[](int index) const {
    return _data[index];
}

std::vector<Feature>& Ndarray::operator[](int index) {
    return _data[index];
}

Ndarray Ndarray::operator+(const Ndarray& other) const {
    EnsureSameShape(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] + other._data[i][j];
        }
    }
    return result;
}

Ndarray Ndarray::operator-(const Ndarray& other) const {
    EnsureSameShape(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] - other._data[i][j];
        }
    }
    return result;
}

Ndarray Ndarray::operator*(const Ndarray& other) const {
    EnsureSameShape(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * other._data[i][j];
        }
    }
    return result;
}

Ndarray Ndarray::operator*(const Ndarray& other) const {
    EnsureSameShape(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] * other._data[i][j];
        }
    }
    return result;
}

Ndarray Ndarray::operator/(const Ndarray& other) const {
    EnsureSameShape(other);
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = _data[i][j] / other._data[i][j];
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