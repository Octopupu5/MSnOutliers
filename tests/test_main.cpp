#include <gtest/gtest.h>
#include "ML/DBSCAN/DBSCAN.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(MetricsTest, mse) {
    Matrix n(1, 1);
    ASSERT_NEAR(0.0, 0.00001, 0.0001);
}

TEST(MetricsTest, mae) {
    // DBSCAN n(0, 0);
    ASSERT_NEAR(0.0, 0.00001, 0.000001);
}