#include "LeastSquares.hpp"
namespace CP {
    Matrix LeastSquaresMethod::compute() {
        return (featuresMatrix_.T() * featuresMatrix_).Inv() * featuresMatrix_.T() * targetMatrix_;
    }
} // namespace CP