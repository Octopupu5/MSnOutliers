#include <random>
#include "COMMON/Ndarray.hpp"
#include "DISTRIBUTIONS/ErrorDistributions.hpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    Ndarray array(5, 5);
    std::cout << 1 << '\n';
    return 0;
}