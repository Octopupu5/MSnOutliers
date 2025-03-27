#ifndef CP_BACK_LEASTSQUARES_H
#define CP_BACK_LEASTSQUARES_H

#include "../COMMON/FileParser.hpp"
#include "StatsMethod.hpp"

namespace CP {
    using RData = RegressionData;

    class LeastSquaresMethod : public StatsMethod {
    public:
        explicit LeastSquaresMethod(const RData& data): StatsMethod(data){}
        Matrix compute() override;
    };
} // namespace CP

#endif