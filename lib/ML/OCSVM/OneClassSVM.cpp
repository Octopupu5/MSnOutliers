#include "OneClassSVM.hpp"

OneClassSVM::OneClassSVM(float nu, float gamma)
    : nu(nu), gamma(gamma), rho(0.0) {}

void OneClassSVM::Fit(const Matrix& data) {
    size_t rows = data.Rows();
    // матрица грамма бай бай
    Matrix kernelMatrix(rows, rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            kernelMatrix.At(i, j) = Kernel(data.RowMatrix(i), data.RowMatrix(j));
        }
    }

    // пми - методы оптимизации
    Matrix alphas(rows, 1);
    alphas.Fill(1.0 / rows);

    // select sup vectors and coefs
    // better to filter based on alphas > 0 threshold
    supportVectors = data;
    // same shit here
    coefficients = alphas;

    // offset
    DecisionBoundary(data);
}

Matrix OneClassSVM::Predict(const Matrix& data) const {
    size_t nSamples = data.Rows();
    Matrix predictions(nSamples, 1);

    for (size_t i = 0; i < nSamples; ++i) {
        double decision = 0.0;
        for (size_t j = 0; j < supportVectors.Rows(); ++j) {
            decision += coefficients.At(j, 0).Value() * Kernel(supportVectors.RowMatrix(i), supportVectors.RowMatrix(j));
        }
        decision -= rho;
        predictions.At(i, 0) = (decision >= 0) ? 1 : -1;
    }

    return predictions;
}

double OneClassSVM::Kernel(const Matrix& x1, const Matrix& x2) const {
    return std::exp(-gamma * std::pow(x1.L2Norm(x2), 2));
}

void OneClassSVM::DecisionBoundary(const Matrix& data) {
    double sum = 0.0;
    for (size_t i = 0; i < supportVectors.Rows(); ++i) {
        double decision = 0.0;
        for (size_t j = 0; j < supportVectors.Rows(); ++j) {
            decision += coefficients.At(j, 0).Value() * Kernel(supportVectors.RowMatrix(i), supportVectors.RowMatrix(j));
        }
        sum += decision;
    }
    rho = sum / supportVectors.Rows();
}