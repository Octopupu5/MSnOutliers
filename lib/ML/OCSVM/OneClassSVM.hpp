#ifndef ONE_CLASS_SVM_HPP
#define ONE_CLASS_SVM_HPP

#include "Ndarray.hpp"
#include <vector>

class OneClassSVM {
public:
    OneClassSVM(float nu, float gamma);
    void Fit(const Ndarray& data);

    // -1 = anomaly; 1 = inlier
    Ndarray Predict(const Ndarray& data) const;

private:
    // radial basis func kernel
    double Kernel(const Ndarray& x1, const Ndarray& x2) const;

    // decision boundary offset (rho)
    void DecisionBoundary(const Ndarray& data);

private:
    // regularization
    float nu;
    // kernel coef
    float gamma;
    Ndarray supportVectors;
    // coefs for supvectors
    Ndarray coefficients;
    // offset for decision boundary
    double rho;
};

#endif // ONE_CLASS_SVM_HPP