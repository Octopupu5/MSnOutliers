add_executable(main test_ndarray.cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})

set_target_properties(
    main
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)