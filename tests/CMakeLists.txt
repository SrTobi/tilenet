

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/tests")

set(test_settings "${CMAKE_CURRENT_SOURCE_DIR}/test_settings.hpp")

include_directories(.)
add_subdirectory(library-test)
add_subdirectory(api-test)