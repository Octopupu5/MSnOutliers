#include <random>
#include "COMMON/Matrix.hpp"
#include "DISTRIBUTIONS/ErrorDistributions.hpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    Matrix array(5, 5);
    std::cout << 1 << '\n';
    return 0;
}