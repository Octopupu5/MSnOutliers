#ifndef ONE_CLASS_SVM_HPP
#define ONE_CLASS_SVM_HPP

#include "../../COMMON/Matrix.hpp"

class OneClassSVM {
public:
    OneClassSVM(float nu, float gamma);
    void Fit(const Matrix& data);

    // -1 = anomaly; 1 = inlier
    Matrix Predict(const Matrix& data) const;

private:
    // radial basis func kernel
    double Kernel(const Matrix& x1, const Matrix& x2) const;

    // decision boundary offset (rho)
    void DecisionBoundary(const Matrix& data);
    // regularization
    float nu;
    // kernel coef
    float gamma;
    Matrix supportVectors;
    // coefs for supvectors
    Matrix coefficients;
    // offset for decision boundary
    double rho;
};

#endif // ONE_CLASS_SVM_HPP