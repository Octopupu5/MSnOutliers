#ifndef Matrix_HPP
#define Matrix_HPP

#include "Feature.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>

class Matrix {
public:
    Matrix();
    Matrix(size_t cols);
    Matrix(size_t rows, size_t cols, double scalar = 0.0);
    Matrix(const std::vector<std::vector<Feature>>& data);

    Matrix RowMatrix(size_t index) const;
    std::vector<Feature> operator[](int index) const;
    std::vector<Feature>& operator[](int index);

    void AddColumns(const Matrix& other);
    void AddRows(const Matrix& other);

    Feature& At(size_t row, size_t col);
    const Feature& At(size_t row, size_t col) const;

    size_t Rows() const;
    size_t Cols() const;
    std::pair<size_t, size_t> Shape() const;

    void Fill(double scalar);
    // void RandomFill(ErrorDistributions::DistributionType type, double param1, double param2, std::mt19937& gen);

    Matrix T() const;
    Matrix Inv() const;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator/(const Matrix& other) const;
    Matrix operator+(double scalar) const;
    Matrix operator-(double scalar) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator/=(const Matrix& other);
    Matrix& operator+=(double scalar);
    Matrix& operator-=(double scalar);
    Matrix& operator*=(double scalar);
    Matrix& operator/=(double scalar);
    
    Matrix Matmul(const Matrix& other) const;
    
    Matrix ColumnWiseMean() const;
    Matrix RowWiseMean() const;
    Matrix ColumnWiseSum() const;
    Matrix RowWiseSum() const;
    double Sum() const;

    Matrix Sqrt() const;
    Matrix Abs() const;
    Matrix Pow(double power) const;

    double L1Norm(const Matrix& other) const;
    double L2Norm(const Matrix& other) const;

    void Print() const;

private:
    size_t _rows, _cols;
    std::vector<std::vector<Feature>> _data;

    void CheckIndices(size_t row, size_t col) const;
    void EnsureSameShape(const Matrix& other) const;
    void EnsureShapeForOperator(const Matrix& other) const;
};

#endif // Matrix_HPP