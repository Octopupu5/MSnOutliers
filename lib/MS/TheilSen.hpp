#ifndef CP_2025_THEIL_SEN_H
#define CP_2025_THEIL_SEN_H

#include "StatsMethod.hpp"

namespace CP {
    namespace MS {
        using Vec = Eigen::VectorXd;
        using Mat = Eigen::MatrixXd;
        class TheilSen : public StatsMethod {
        public:
            explicit TheilSen(const RData& data): StatsMethod(data) {}
            Vec compute() override;
        };
    } // namespace MS;
} // namespace CP;


#endif