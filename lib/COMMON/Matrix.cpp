#include "Matrix.hpp"

namespace CP {
    namespace Common {
        Matrix::Matrix(): _rows(0), _cols(0) {}
        Matrix::Matrix(size_t cols) : _rows(0), _cols(cols) {}
        Matrix::Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols), _data(rows, std::vector<Feature>(cols, 0.0)) {}
        Matrix::Matrix(size_t rows, size_t cols, double scalar) : _rows(rows), _cols(cols), _data(rows, std::vector<Feature>(cols, scalar)) {}

        Matrix::Matrix(const std::vector<std::vector<Feature>>& data) : _rows(data.size()), _cols(data.empty() ? 0 : data[0].size()), _data(data) {
            for (const auto& row : data) {
                if (row.size() != _cols) {
                    throw std::invalid_argument("All rows must have the same number of columns");
                }
            }
        }

        void Matrix::CheckIndices(size_t row, size_t col) const {
            if (row >= _rows || col >= _cols || row < 0 || col < 0) {
                throw std::out_of_range("Index out of bounds");
            }
        }

        void Matrix::EnsureSameShape(const Matrix& other) const {
            if (_rows != other._rows || _cols != other._cols) {
                throw std::invalid_argument("Arrays must have the same shape for this operation");
            }
        }

        void Matrix::EnsureShapeForOperator(const Matrix& other) const {
            // if ((_rows != other._rows && _cols != other._cols)) {
            //     if ((_rows == other._rows && other._cols != 1) || (other._rows != 1 && _cols == other._cols)) {
            //         throw std::invalid_argument("Arrays must have at least identical rows or cols amount for this operation");
            //     }
            //     throw std::invalid_argument("Arrays must have at least identical rows or cols amount for this operation");
            // }
        }

        Feature& Matrix::At(size_t row, size_t col) {
            CheckIndices(row, col);
            return _data[row][col];
        }

        const Feature& Matrix::At(size_t row, size_t col) const {
            CheckIndices(row, col);
            return _data[row][col];
        }

        size_t Matrix::Rows() const {
            return _rows;
        }

        size_t Matrix::Cols() const {
            return _cols;
        }

        std::pair<size_t, size_t> Matrix::Shape() const {
            return {_rows, _cols};
        }

        Matrix Matrix::T() const {
            Matrix result = Matrix(_cols, _rows);
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[j][i] = _data[i][j];
                }
            }
            return result;
        }

        Matrix Matrix::Inv() const {
            if (_rows != _cols) {
                throw std::invalid_argument("Matrix must be square for inversion");
            }
            
            Matrix augmented(_rows, _cols * 2);
            
            for (size_t i = 0; i < _rows; i++) {
                for (size_t j = 0; j < _cols; j++) {
                    augmented.At(i, j) = At(i, j);
                }
                augmented.At(i, i + _cols) = 1.0;
            }
            
            for (size_t i = 0; i < _rows; i++) {
                size_t max_row = i;
                for (size_t k = i + 1; k < _rows; k++) {
                    if (std::abs(augmented.At(k, i).Value()) > std::abs(augmented.At(max_row, i).Value())) {
                        max_row = k;
                    }
                }
                
                if (std::abs(augmented.At(max_row, i).Value()) < 1e-10) {
                    throw std::runtime_error("Matrix is singular and cannot be inverted");
                }
                
                if (max_row != i) {
                    for (size_t j = 0; j < 2 * _cols; j++) {
                        std::swap(augmented.At(i, j), augmented.At(max_row, j));
                    }
                }
                
                double pivot = augmented.At(i, i).Value();
                for (size_t j = 0; j < 2 * _cols; j++) {
                    augmented.At(i, j) = augmented.At(i, j).Value() / pivot;
                }
                
                for (size_t k = 0; k < _rows; k++) {
                    if (k != i) {
                        double factor = augmented.At(k, i).Value();
                        for (size_t j = 0; j < 2 * _cols; j++) {
                            augmented.At(k, j) = augmented.At(k, j).Value() - factor * augmented.At(i, j).Value();
                        }
                    }
                }
            }
            
            Matrix inverse(_rows, _cols);
            for (size_t i = 0; i < _rows; i++) {
                for (size_t j = 0; j < _cols; j++) {
                    inverse.At(i, j) = augmented.At(i, j + _cols);
                }
            }
            
            return inverse;
        }

        Matrix Matrix::Matmul(const Matrix& other) const {
            if (_cols != other._rows) {
                throw std::invalid_argument("Shapes are not aligned for matrix multiplication");
            }
            Matrix result(_rows, other._cols);
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < other._cols; ++j) {
                    for (size_t k = 0; k < _cols; ++k) {
                        result._data[i][j] += _data[i][k] * other._data[k][j];
                    }
                }
            }
            return result;
        }

        Matrix Matrix::ColumnWiseMean() const {
            return ColumnWiseSum() / _rows; 
        }

        Matrix Matrix::RowWiseMean() const {
            return RowWiseSum() / _cols;
        }

        // 1 2 3
        // 4 5 6
        // 1 2 5
        // --->>
        // 6 9 14
        Matrix Matrix::ColumnWiseSum() const {
            Matrix result(1, _cols);
            for (size_t j = 0; j < _cols; ++j) {
                double accumulated = 0.0;
                for (size_t i = 0; i < _rows; ++i) {
                    accumulated += _data[i][j].Value();
                }
                result.At(0, j).SetValue(accumulated);
            }
            return result;
        }

        Matrix Matrix::RowWiseSum() const {
            Matrix result(_rows, 1);
            for (size_t i = 0; i < _rows; ++i) {
                double accumulated = 0.0;
                for (size_t j = 0; j < _cols; ++j) {
                    accumulated += _data[i][j].Value();
                }
                result.At(i, 0).SetValue(accumulated);
            }
            return result;
        }

        double Matrix::Sum() const {
            double result = 0.0;
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result += _data[i][j].Value();
                }
            }
            return result;
        }

        template <typename Function>
        typename std::enable_if<std::is_invocable_r<Feature, Function, Feature>::value, Matrix>::type Matrix::Apply(Function&& f) const {
            Matrix result(_rows, _cols);

            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[i][j] = f(_data[i][j]);
                }
            }

            return result;
        }

        template <typename Function> typename std::enable_if<std::is_invocable_r<Feature, Function, double>::value, Matrix>::type Matrix::Apply(Function&& f) const {
            Matrix result(_rows, _cols);

            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[i][j] = f(_data[i][j].Value());
                }
            }

            return result;
        }

        Matrix Matrix::Sqrt() const {
            Matrix result(_rows, _cols);
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[i][j] = std::sqrt(_data[i][j].Value());
                }
            }
            return result;
        }

        Matrix Matrix::Abs() const {
            Matrix result(_rows, _cols);
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[i][j] = std::fabs(_data[i][j].Value());
                }
            }
            return result;
        }

        Matrix Matrix::Pow(double power) const {
            Matrix result(_rows, _cols);
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result._data[i][j] = std::pow(_data[i][j].Value(), power);
                }
            }
            return result;
        }

        double Matrix::L1Norm(const Matrix& other) const {
            EnsureSameShape(other);
            double result = 0;
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result += std::fabs((_data[i][j] - other._data[i][j]).Value());
                }
            }
            return std::sqrt(result);
        }

        double Matrix::L2Norm(const Matrix& other) const {
            EnsureSameShape(other);
            Feature result = 0;
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result += (_data[i][j] - other._data[i][j]) * (_data[i][j] - other._data[i][j]);
                }
            }
            return std::sqrt(result.Value());
        }

        void Matrix::Fill(double scalar) {
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    _data[i][j].SetValue(scalar);
                }
            }
        }

        Matrix Matrix::RowMatrix(size_t index) const {
            if (index >= _rows) {
                throw std::invalid_argument("Row size is less, invalid index");
            }
            return Matrix({_data[index]});
        }

        Matrix Matrix::ColumnMatrix(size_t index) const {
            if (index >= _cols) {
                throw std::invalid_argument("Column size is less, invalid index");
            }
            Matrix result(_rows, 1);
            for (size_t i = 0; i < _rows; ++i) {
                result._data[i][0] = _data[i][index];
            }
            return result;
        }

        void Matrix::AddColumns(const Matrix& other) {
            if (_rows != other._rows) {
                throw std::invalid_argument("Row size should be the same");
            }
            _cols += other._cols;
            for (size_t i = 0; i < _rows; ++i) {
                _data[i].insert(_data[i].end(), other._data[i].begin(), other._data[i].end());
            }
        }

        void Matrix::AddRows(const Matrix& other) {
            if (_cols != other._cols) {
                throw std::invalid_argument("Row size should be the same");
            }
            _rows += other._rows;
            _data.insert(_data.end(), other._data.begin(), other._data.end());
        }

        void Matrix::Print() const {
            for (const auto& row : _data) {
                for (Feature value : row) {
                    std::cout << std::setw(10) << value.Value() << " ";
                }
                std::cout << "\n";
            }
        }
    }
}