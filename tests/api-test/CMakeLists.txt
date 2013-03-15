
file(GLOB convert_test_source "convert-test/*.cpp")
set(test_library_module "api-module.cpp")


source_group(tests FILES ${test_settings} ${test_library_module})
source_group(convert-test FILES ${convert_test_source})

add_executable(api-test ${test_library_module} ${convert_test_source} ${test_settings})
target_link_libraries(api-test ${Boost_LIBRARIES})