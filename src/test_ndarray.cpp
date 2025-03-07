#include <random>
#include "../lib/Ndarray.hpp"
#include "../lib/DISTRIBUTIONS/ErrorDistributions.hpp"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    Ndarray array(5, 5);

    array.RandomFill(ErrorDistributions::Normal, 0.0, 1.0, gen);

    array.Print();

    return 0;
}