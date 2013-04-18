file(GLOB link_test_source "link-test/*.cpp")
source_group("tests\\link-test" FILES ${link_test_source})

set(test_library_module "library-module.cpp")
include_directories("${PROJECT_SOURCE_DIR}/library")


source_group("tests" FILES ${test_settings} ${test_library_module})
source_group("library" FILES ${library_all_source})

add_executable(library-test ${test_library_module} ${test_settings} ${library_all_source} ${link_test_source})
target_link_libraries(library-test ${Boost_LIBRARIES})
add_precompiled_header(library-test	${tilenet_library_pch_file})
