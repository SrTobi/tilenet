
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/samples/test)

file(GLOB sample_test_source *.cpp)
add_executable(test ${sample_test_source})
target_link_libraries(test tilenet ${tilenet_dependency_libs})

NEED_MEDIA(test cour.ttf)
NEED_MEDIA(test samples/test)