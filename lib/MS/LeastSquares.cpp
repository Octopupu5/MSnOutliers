#include "LeastSquares.hpp"
namespace CP {
    Matrix LeastSquaresMethod::compute() {

        return (featuresMatrix_.T().Matmul(featuresMatrix_)).Inv().Matmul(featuresMatrix_.T()).Matmul(targetMatrix_);
    }
} // namespace CP