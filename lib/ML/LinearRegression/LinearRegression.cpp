#include "LinearRegression.hpp"

LinearRegression::LinearRegression() : _GDrunner(128, 0.0001, N_ITERS, {100}, [&] (const Matrix& X, const Matrix& y, const Matrix& w) {
    return Matrix(0);
}) {}

void LinearRegression::Fit(const Matrix& X, const Matrix& y) {
    Matrix toFit = X;
    toFit.AddColumns(Matrix(X.Rows(), 1, 1));
    _GDrunner.Run(X, y);
}

Matrix LinearRegression::Predict(const Matrix& X) const {
    Matrix toPredict = X;
    toPredict.AddColumns(Matrix(X.Rows(), 1, 1));
    return toPredict.Matmul(_GDrunner.Weights());
}