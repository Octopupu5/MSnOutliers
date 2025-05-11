#ifndef ERRORDISTRIBUTIONS_HPP
#define ERRORDISTRIBUTIONS_HPP

#include <random>
#include <variant>
#include <cmath>
#include <stdexcept>

namespace CP {
    namespace Distributions {
        class ErrorDistributions {
        public:
            enum DistributionType {Normal, Student, Cauchy, Laplace};
            ErrorDistributions(DistributionType type, double param1 = 0.0, double param2 = 1.0);
            double generate(std::mt19937& gen);
        private:
            DistributionType _type;
            std::variant<std::normal_distribution<>, std::cauchy_distribution<>, std::pair<double, double>> _distribution;
            double laplace_generate(std::mt19937& gen, double mean, double b);
            double student_t_generate(std::mt19937& gen, double degrees_of_freedom);
        };
    }
}

#endif // ERRORDISTRIBUTIONS_HPP