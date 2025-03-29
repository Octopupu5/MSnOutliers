#include "GD.hpp"

namespace CP {
    namespace ML {
        namespace GD {
            GradientDescent::GradientDescent(size_t batchSize, double learningRate, size_t stopCriterion, const std::vector<double>& criterions, GradientFunction func)
            : _batchSize(batchSize), _learningRate(learningRate), _stopCriterion(stopCriterion), _calcGrad(func) {
                size_t i = 0;
                if (N_ITERS & _stopCriterion) {
                    _nIters = (size_t)criterions[i];
                    ++i;
                }
                if (W_DIFF & _stopCriterion) {
                    _wTolerance = criterions[i];
                    ++i;
                }
            }

            void GradientDescent::Run(const Common::Matrix& X, const Common::Matrix& y) {
                _curIter = 1;
                InitializeParams(X.Cols());
                while (_stopCriterion) {
                    const Common::Matrix& prev_w = _w;
                    GradientStep(X, y);
                    if (((N_ITERS & _stopCriterion) && _curIter < _nIters) || ((W_DIFF & _stopCriterion) && _w.L2Norm(prev_w) < _wTolerance)) {
                        break;
                    }
                    ++_curIter;
                }
            }

            void GradientDescent::GradientStep(const Common::Matrix& X, const Common::Matrix& y) {
                _w -= _calcGrad(X, y, _w) * _learningRate;
            }

            void GradientDescent::InitializeParams(size_t nParams) {
                _w = Common::Matrix(nParams, 1);
            }

            Common::Matrix GradientDescent::Weights() const {
                return _w;
            }
        }
    }
}