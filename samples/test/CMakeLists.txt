
file(GLOB sample_test_source *.cpp)
add_executable(test ${sample_test_source})
target_link_libraries(test tilenet ${tilenet_dependency_libs})
