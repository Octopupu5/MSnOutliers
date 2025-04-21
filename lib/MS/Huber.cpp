#include "Huber.hpp"
namespace CP {
    namespace MS {
        double Huber::loss(double rem) const {
            if (std::abs(rem) <= _delta) {
                return 0.5 * rem * rem;
            }
            return  _delta * (std::abs(rem) - 0.5 * _delta);
        }

        double Huber::gradient(double rem) const {
            if (std::abs(rem) <= _delta) {
                return rem;
            }
            return _delta * ((rem > 0) ? 1 : -1);
        }
    } // namespace MS;
} // namespace CP;
