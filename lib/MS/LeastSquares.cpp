#include "LeastSquares.hpp"
namespace CP {
    namespace MS {
        Vec LeastSquaresMethod::compute() {
            return (_featuresMatrix.transpose() * _featuresMatrix).inverse() * _featuresMatrix.transpose() * _targetMatrix;        }
    } //namespace MS;
} // namespace CP;