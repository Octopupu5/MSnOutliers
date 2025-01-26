#include <gtest/gtest.h>
#include "Metrics.hpp"

TEST(MetricsTest, mse) {
    Metrics m;
    std::vector<double> y1({1., 2., 3.});
    std::vector<double> y2({1., 2., 3.});
    ASSERT_NEAR(0., m.meanSquaredError(y1, y2), 0.0001);
}