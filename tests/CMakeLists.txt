set(TESTFILES
    test_main.cpp
)
set(TEST_MAIN tests)
set(TEST_RUNNER_PARAMS "")
add_executable(${TEST_MAIN} ${TESTFILES})

find_package(GTest REQUIRED)
target_link_libraries(${TEST_MAIN} PRIVATE ${LIBRARY_NAME} GTest::GTest GTest::Main)

add_test(
    NAME ${LIBRARY_NAME}.${TEST_MAIN}
    COMMAND ${TEST_MAIN} ${TEST_RUNNER_PARAMS}
)