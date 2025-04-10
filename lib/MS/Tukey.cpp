#include "Tukey.hpp"

namespace CP {
    namespace MS {
        double Tukey::gradient(double rem) const {
            double u = rem / delta_;
            if (std::abs(u) < 1.0) {
                return (1.0 - u * u) * (1.0 - u * u);
            }
            return 0.0;
        }

        double Tukey::loss(double rem) const {
            if (std::abs(rem) <= delta_) {
                double tmp = 1 - (rem / delta_) * (rem / delta_);
                return ((delta_*delta_)/6) * (1 - (tmp * tmp * tmp));
            }
            return ((delta_*delta_)/6);
        } 
    } // namespace MS;
} // namespace CP;


