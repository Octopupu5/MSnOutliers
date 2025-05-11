add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${LIBRARY_NAME})

add_custom_target(clean_outputs
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/src/outputs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/src/outputs
    COMMENT "Cleaning all contents of the outputs directory"
)

add_custom_target(clean_configs
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/src/configs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/src/configs
    COMMENT "Cleaning all contents of the configs directory"
)

set_target_properties(
    main
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)