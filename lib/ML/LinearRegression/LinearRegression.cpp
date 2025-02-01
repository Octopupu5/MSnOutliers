#include "LinearRegression.hpp"

LinearRegression::LinearRegression() : _GDrunner(128, 0.0001, N_ITERS, {100}, [&] (const Ndarray& X, const Ndarray& y, const Ndarray& w) {
    return Ndarray(0);
}) {}

void LinearRegression::Fit(const Ndarray& X, const Ndarray& y) {
    Ndarray toFit = X;
    toFit.AddColumns(Ndarray(X.Rows(), 1, 1));
    _GDrunner.Run(X, y);
}

Ndarray LinearRegression::Predict(const Ndarray& X) const {
    Ndarray toPredict = X;
    toPredict.AddColumns(Ndarray(X.Rows(), 1, 1));
    return toPredict.Matmul(_GDrunner.Weights());
}