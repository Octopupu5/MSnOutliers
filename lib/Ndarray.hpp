#ifndef NDARRAY_HPP
#define NDARRAY_HPP

#include "Feature.hpp"
#include "DISTRIBUTIONS/ErrorDistributions.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iomanip>

class Ndarray {
public:
    Ndarray();
    Ndarray(size_t cols);
    Ndarray(size_t _rows, size_t cols, double scalar);
    Ndarray(const std::vector<std::vector<Feature>>& data);

    Ndarray RowNdarray(size_t index) const;
    std::vector<Feature> operator[](int index) const;
    std::vector<Feature>& operator[](int index);

    void AddColumns(const Ndarray& other);
    void AddRows(const Ndarray& other);

    Feature& At(size_t row, size_t col);
    const Feature& At(size_t row, size_t col) const;

    size_t Rows() const;
    size_t Cols() const;
    std::pair<size_t, size_t> Shape() const;

    void Fill(double scalar);
    void RandomFill(ErrorDistributions::DistributionType type, double param1, double param2, std::mt19937& gen);

    Ndarray Transpose() const;
    Ndarray operator+(const Ndarray& other) const;
    Ndarray operator-(const Ndarray& other) const;
    Ndarray operator*(const Ndarray& other) const;
    Ndarray operator/(const Ndarray& other) const;
    Ndarray operator+(double scalar) const;
    Ndarray operator-(double scalar) const;
    Ndarray operator*(double scalar) const;
    Ndarray operator/(double scalar) const;

    Ndarray& operator+=(const Ndarray& other);
    Ndarray& operator-=(const Ndarray& other);
    Ndarray& operator*=(const Ndarray& other);
    Ndarray& operator/=(const Ndarray& other);
    Ndarray& operator+=(double scalar);
    Ndarray& operator-=(double scalar);
    Ndarray& operator*=(double scalar);
    Ndarray& operator/=(double scalar);
    
    Ndarray Matmul(const Ndarray& other) const;
    
    Ndarray ColumnWiseMean() const;
    Ndarray RowWiseMean() const;
    Ndarray ColumnWiseSum() const;
    Ndarray RowWiseSum() const;
    double Sum() const;

    Ndarray Sqrt() const;
    Ndarray Abs() const;

    double L1Norm(const Ndarray& other) const;
    double L2Norm(const Ndarray& other) const;

    void Print() const;

private:
    size_t _rows, _cols;
    std::vector<std::vector<Feature>> _data;

    void CheckIndices(size_t row, size_t col) const;
    void EnsureSameShape(const Ndarray& other) const;
    void EnsureShapeForOperator(const Ndarray& other) const;
};

#endif // NDARRAY_HPP