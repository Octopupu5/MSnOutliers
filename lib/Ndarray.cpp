#include "Ndarray.hpp"

Ndarray::Ndarray(): _rows(0), _cols(0) {}
Ndarray::Ndarray(size_t cols) : _rows(0), _cols(cols) {}
Ndarray::Ndarray(size_t _rows, size_t cols, double scalar = 0.0)
        : _rows(_rows), _cols(cols), _data(_rows, std::vector<Feature>(cols, scalar)) {}

Ndarray::Ndarray(const std::vector<std::vector<Feature>>& data)
    : _rows(data.size()), _cols(data.empty() ? 0 : data[0].size()), _data(data) {
    for (const auto& row : data) {
        if (row.size() != _cols) {
            throw std::invalid_argument("All rows must have the same number of columns");
        }
    }
}

void Ndarray::CheckIndices(size_t row, size_t col) const {
    if (row >= _rows || col >= _cols) {
        throw std::out_of_range("Index out of bounds");
    }
}

void Ndarray::EnsureSameShape(const Ndarray& other) const {
    if (_rows != other._rows || _cols != other._cols) {
        throw std::invalid_argument("Arrays must have the same shape for this operation");
    }
}

void Ndarray::EnsureShapeForOperator(const Ndarray& other) const {
    if ((_rows != other._rows && _cols != other._cols)) {
        throw std::invalid_argument("Arrays must have at least identical rows or cols amount for this operation");
    }
    if ((_rows == other._rows && other._cols != 1) || (other._rows != 1 && _cols == other._cols)) {
        throw std::invalid_argument("Arrays must have at least identical rows or cols amount for this operation");
    }
}

Feature& Ndarray::At(size_t row, size_t col) {
    CheckIndices(row, col);
    return _data[row][col];
}

const Feature& Ndarray::At(size_t row, size_t col) const {
    CheckIndices(row, col);
    return _data[row][col];
}

size_t Ndarray::Rows() const {
    return _rows;
}

size_t Ndarray::Cols() const {
    return _cols;
}

std::pair<size_t, size_t> Ndarray::Shape() const {
    return {_rows, _cols};
}

Ndarray Ndarray::Transpose() const {
    Ndarray result = Ndarray(_cols, _rows);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[j][i] = _data[i][j];
        }
    }
    return result;
}

Ndarray Ndarray::Matmul(const Ndarray& other) const {
    if (_cols != other._rows) {
        throw std::invalid_argument("Shapes are not aligned for matrix multiplication");
    }
    Ndarray result(_rows, other._cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < other._cols; ++j) {
            for (size_t k = 0; k < _cols; ++k) {
                result._data[i][j] += _data[i][k] * other._data[k][j];
            }
        }
    }
    return result;
}

Ndarray Ndarray::ColumnWiseMean() const {
    return ColumnWiseSum() / _rows; 
}

Ndarray Ndarray::RowWiseMean() const {
    return RowWiseSum() / _cols;
}

// 1 2 3
// 4 5 6
// 1 2 5
// --->>
// 6 9 14
Ndarray Ndarray::ColumnWiseSum() const {
    Ndarray result(1, _cols);
    for (size_t j = 0; j < _cols; ++j) {
        double accumulated = 0.0;
        for (size_t i = 0; i < _rows; ++i) {
            accumulated += _data[i][j].Value();
        }
        result.At(0, j).SetValue(accumulated);
    }
    return result;
}

Ndarray Ndarray::RowWiseSum() const {
    Ndarray result(_rows, 1);
    for (size_t i = 0; i < _rows; ++i) {
        double accumulated = 0.0;
        for (size_t j = 0; j < _cols; ++j) {
            accumulated += _data[i][j].Value();
        }
        result.At(i, 0).SetValue(accumulated);
    }
    return result;
}

double Ndarray::Sum() const {
    double result = 0.0;
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result += _data[i][j].Value();
        }
    }
    return result;
}

Ndarray Ndarray::Sqrt() const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = std::sqrt(_data[i][j].Value());
        }
    }
    return result;
}

Ndarray Ndarray::Abs() const {
    Ndarray result(_rows, _cols);
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result._data[i][j] = std::fabs(_data[i][j].Value());
        }
    }
    return result;
}

double Ndarray::L1Norm(const Ndarray& other) const {
    EnsureSameShape(other);
    double result = 0;
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result += std::fabs((_data[i][j] - other._data[i][j]).Value());
        }
    }
    return std::sqrt(result);
}

double Ndarray::L2Norm(const Ndarray& other) const {
    EnsureSameShape(other);
    Feature result = 0;
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result += (_data[i][j] - other._data[i][j]) * (_data[i][j] - other._data[i][j]);
        }
    }
    return std::sqrt(result.Value());
}

void Ndarray::Fill(double scalar) {
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            _data[i][j].SetValue(scalar);
        }
    }
}

Ndarray Ndarray::RowNdarray(size_t index) const {
    if (index >= _rows) {
        throw std::invalid_argument("Row size is less, invalid index");
    }
    return Ndarray({_data[index]});
}

void Ndarray::AddColumns(const Ndarray& other) {
    if (_rows != other._rows) {
        throw std::invalid_argument("Row size should be the same");
    }
    _cols += other._cols;
    for (size_t i = 0; i < _rows; ++i) {
        _data[i].insert(_data[i].end(), other._data[i].begin(), other._data[i].end());
    }
}

void Ndarray::AddRows(const Ndarray& other) {
    if (_cols != other._cols) {
        throw std::invalid_argument("Row size should be the same");
    }
    _rows += other._rows;
    _data.insert(_data.end(), other._data.begin(), other._data.end());
}

void Ndarray::Print() const {
    for (const auto& row : _data) {
        for (Feature value : row) {
            std::cout << std::setw(10) << value.Name() << " ";
        }
        std::cout << "\n";
    }
}