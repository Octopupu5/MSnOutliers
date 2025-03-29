#include "LeastSquares.hpp"
namespace CP {
    namespace MS {
        Common::Matrix LeastSquaresMethod::compute() {
            return (_featuresMatrix.T().Matmul(_featuresMatrix)).Inv().Matmul(_featuresMatrix.T()).Matmul(_targetMatrix);
        }
    }
} // namespace CP