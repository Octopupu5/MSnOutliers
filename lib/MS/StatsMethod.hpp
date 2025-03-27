#ifndef CP_2025_STATSMETHOD_H
#define CP_2025_STATSMETHOD_H

#include "../COMMON/FileParser.hpp"
#include "../COMMON/Matrix.hpp"

namespace CP {
    using RData = RegressionData;
    class StatsMethod {
    public:
        explicit StatsMethod(const RData& data);
        virtual Matrix compute() = 0;

    protected:
        Matrix featuresMatrix_;
        Matrix targetMatrix_;
    };
}


#endif //CP_2025_STATSMETHOD_H