file(GLOB ptr_test_source "ptr-test/*.cpp")
set(test_library_module "library-module.cpp")
include_directories("${PROJECT_SOURCE_DIR}/library/include")


source_group(tests FILES ${test_settings} ${test_library_module})
source_group(ptrtest FILES ${ptr_test_source})
source_group(library FILES ${library_all_source})

add_executable(library-test ${test_library_module} ${ptr_test_source} ${test_settings} ${library_all_source})
add_precompiled_header(library-test	${tilenet_library_pch_file})