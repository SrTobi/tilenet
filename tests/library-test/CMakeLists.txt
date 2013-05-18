

macro(add_test_component name)

	file(GLOB test_source "${name}/*.cpp")
	source_group("tests\\${name}" FILES ${test_source})

	set(library_test_source "${library_test_source};${test_source}")

endmacro(add_test_component)


add_test_component("link-test")
add_test_component("lexical_convert-test")

set(test_library_module "library-module.cpp")
include_directories("${PROJECT_SOURCE_DIR}/library")


source_group("tests" FILES ${test_settings} ${test_library_module})
source_group("library" FILES ${library_all_source})

add_executable(library-test ${test_library_module} ${test_settings} ${library_all_source} ${library_test_source})
target_link_libraries(library-test ${tilenet_dependency_libs})
add_precompiled_header(library-test	${tilenet_library_pch_file})
