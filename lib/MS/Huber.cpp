#include "Huber.hpp"
namespace CP {
    namespace MS {
        double Huber::loss(double rem) const {
            if (std::abs(rem) <= delta_) {
                return 0.5 * rem * rem;
            }
            return  delta_ * (std::abs(rem) - 0.5 * delta_);
        }

        double Huber::gradient(double rem) const {
            if (std::abs(rem) <= delta_) {
                return rem;
            }
            return delta_ * ((rem > 0) ? 1 : -1);
        }
    } // namespace MS;
} // namespace CP;
