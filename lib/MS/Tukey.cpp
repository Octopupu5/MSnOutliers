#include "Tukey.hpp"

namespace CP {
    namespace MS {
        double Tukey::gradient(double rem) const {
            double u = rem / _delta;
            if (std::abs(u) <= 1.0) {
                return rem * (1.0 - u * u) * (1.0 - u * u);
            }
            return 0.0;
        }

        double Tukey::loss(double rem) const {
            if (std::abs(rem) <= _delta) {
                double tmp = 1 - (rem / _delta) * (rem / _delta);
                return ((_delta*_delta)/6) * (1 - (tmp * tmp * tmp));
            }
            return ((_delta*_delta)/6);
        } 
    } // namespace MS;
} // namespace CP;


