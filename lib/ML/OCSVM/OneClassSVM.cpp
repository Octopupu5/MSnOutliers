#include "OneClassSVM.hpp"

OneClassSVM::OneClassSVM(float nu, float gamma)
    : nu(nu), gamma(gamma), rho(0.0) {}

void OneClassSVM::fit(const Ndarray& data) {
    size_t nSamples = data.shape(0);
    size_t nFeatures = data.shape(1);

    // матрица грамма бай бай
    Ndarray kernelMatrix(nSamples, nSamples);
    for (size_t i = 0; i < nSamples; ++i) {
        for (size_t j = 0; j < nSamples; ++j) {
            kernelMatrix(i, j) = kernel(data[i], data[j]);
        }
    }

    // пми - методы оптимизации
    Ndarray alphas(nSamples, 1);
    alphas.fill(1.0 / nSamples);

    // select sup vectors and coefs
    // better to filter based on alphas > 0 threshold
    supportVectors = data;
    // same shit here
    coefficients = alphas;

    // offset
    calculateDecisionBoundary(data);
}

Ndarray OneClassSVM::predict(const Ndarray& data) const {
    size_t nSamples = data.Rows();
    Ndarray predictions(nSamples, 1);

    for (size_t i = 0; i < nSamples; ++i) {
        double decision = 0.0;
        for (size_t j = 0; j < supportVectors.Rows(); ++j) {
            decision += coefficients(j, 0) * kernel(data.row(i), supportVectors.row(j));
        }
        decision -= rho;
        predictions(i, 0) = (decision >= 0) ? 1 : -1;
    }

    return predictions;
}

double OneClassSVM::kernel(const Ndarray& x1, const Ndarray& x2) const {
    // RBF
    double distanceSquared = 0.0;
    for (size_t i = 0; i < x1.shape(0); ++i) {
        distanceSquared += std::pow(x1(i) - x2(i), 2);
    }
    return std::exp(-gamma * distanceSquared);
}

void OneClassSVM::calculateDecisionBoundary(const Ndarray& data) {
    double sum = 0.0;
    for (size_t i = 0; i < supportVectors.shape(0); ++i) {
        double decision = 0.0;
        for (size_t j = 0; j < supportVectors.shape(0); ++j) {
            decision += coefficients(j, 0) * kernel(supportVectors.row(i), supportVectors.row(j));
        }
        sum += decision;
    }
    rho = sum / supportVectors.shape(0);
}