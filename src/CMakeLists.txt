add_executable(main main.cpp)
add_executable(generation test_generation.cpp)
add_executable(writeAndDraw writeAndDraw.cpp)

add_executable(iforest iforest_test.cpp)
add_executable(dbscan dbscan_test.cpp)
add_executable(svm svm_test.cpp)

target_link_libraries(main PRIVATE ${LIBRARY_NAME})
target_link_libraries(generation PRIVATE ${LIBRARY_NAME})
target_link_libraries(writeAndDraw PRIVATE ${LIBRARY_NAME})

target_link_libraries(iforest PRIVATE ${LIBRARY_NAME})
target_link_libraries(dbscan PRIVATE ${LIBRARY_NAME})
target_link_libraries(svm PRIVATE ${LIBRARY_NAME})

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
    main generation writeAndDraw
      PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)