
file(GLOB function_test_source "function-tests/*.cpp")
set(test_library_module "api-module.cpp")


source_group(tests FILES ${test_settings} ${test_library_module})
source_group(function-test FILES ${function_test_source})

add_executable(api-test ${test_library_module} ${function_test_source} ${test_settings})
target_link_libraries(api-test ${tilenet_dependency_libs} tilenet)