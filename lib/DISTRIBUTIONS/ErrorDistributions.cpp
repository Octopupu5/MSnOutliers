#include "ErrorDistributions.hpp"

namespace CP {
    namespace Distributions {
        ErrorDistributions::ErrorDistributions(DistributionType type, double param1, double param2)
            : _type(type) {
            switch (_type) {
                case Normal:
                    _distribution = std::normal_distribution<>(param1, param2);
                    break;
                case Student:
                    _distribution = std::pair<double, double>{param1, 0.0};
                    break;
                case Cauchy:
                    _distribution = std::cauchy_distribution<>(param1, param2);
                    break;
                case Laplace:
                    _distribution = std::make_pair(param1, param2);
                    break;
                default:
                    break;
            }
        }

        double ErrorDistributions::generate(std::mt19937& gen) {
            switch (_type) {
                case Normal:
                    return std::get<std::normal_distribution<>>(_distribution)(gen);
                case Student:
                    return student_t_generate(gen, std::get<std::pair<double, double>>(_distribution).first);
                case Cauchy:
                    return std::get<std::cauchy_distribution<>>(_distribution)(gen);
                case Laplace: {
                        auto [mean, b] = std::get<std::pair<double, double>>(_distribution);
                        return laplace_generate(gen, mean, b);
                    }
                default:
                    throw std::runtime_error("aboba");
            }
        }

        double ErrorDistributions::laplace_generate(std::mt19937& gen, double mean, double b) {
            std::uniform_real_distribution<> uniform(0.0, 1.0);
            double u = uniform(gen) - 0.5;
            return mean - b * (u < 0 ? 1 : -1) * std::log(1 - 2 * std::abs(u));
        }

        double ErrorDistributions::student_t_generate(std::mt19937& gen, double degrees_of_freedom) {
            std::normal_distribution<> normal(0.0, 1.0);
            std::chi_squared_distribution<> chi_squared(degrees_of_freedom);
            double normal_sample = normal(gen);
            double chi_squared_sample = chi_squared(gen);
            return normal_sample / std::sqrt(chi_squared_sample / degrees_of_freedom);
        }
    }
}