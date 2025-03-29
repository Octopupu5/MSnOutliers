#ifndef CP_2025_STATSMETHOD_H
#define CP_2025_STATSMETHOD_H

#include "../COMMON/FileParser.hpp"
#include "../COMMON/Matrix.hpp"

namespace CP {
    namespace MS {
        using RData = Common::RegressionData;
        class StatsMethod {
        public:
            explicit StatsMethod(const RData& data);
            virtual Common::Matrix compute() = 0;

        protected:
            Common::Matrix _featuresMatrix;
            Common::Matrix _targetMatrix;
        };
    }
}


#endif //CP_2025_STATSMETHOD_H