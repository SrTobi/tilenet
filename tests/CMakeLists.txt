

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/tests")

set(test_settings "${CMAKE_CURRENT_SOURCE_DIR}/test_settings.hpp")
add_definitions(-DTILENET_TESTS)

include_directories(.)
add_subdirectory(library-test)
add_subdirectory(api-test)