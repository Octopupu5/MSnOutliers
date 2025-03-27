add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME} nlohmann_json::nlohmann_json)

set_target_properties(
    main
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)