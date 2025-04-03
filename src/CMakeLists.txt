add_executable(main main.cpp)
add_executable(generation test_generation.cpp)
add_executable(writeAndDraw writeAndDraw.cpp)

target_link_libraries(main PRIVATE ${LIBRARY_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(generation PRIVATE ${LIBRARY_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(writeAndDraw PRIVATE ${LIBRARY_NAME} nlohmann_json::nlohmann_json)

set_target_properties(
    main generation writeAndDraw
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)