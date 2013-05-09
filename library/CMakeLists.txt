# macro for adding source
macro(add_current_directory_to_library_source)

	# Glob files of this scope
	file(GLOB library_current_source "*.cpp")
	file(GLOB library_current_includes "*.hpp")

	# add source to library_source
	set(library_source "${library_source};${library_current_source};${library_current_includes}" PARENT_SCOPE)
endmacro(add_current_directory_to_library_source)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

#include subdirectories
add_subdirectory("client")
add_subdirectory("network")
add_subdirectory("server")
add_subdirectory("utils")

#this directory also contains source files
file(GLOB library_current_source "*.cpp")
file(GLOB library_current_includes "*.hpp")
set(library_source "${library_source};${library_current_source};${library_current_includes}")

#set all source files for the global scope
set(library_all_source ${library_source} PARENT_SCOPE)


add_library(tilenet ${library_source} ${tilenet_include_file} )
target_link_libraries(tilenet ${Boost_LIBRARIES})

add_precompiled_header(tilenet	${tilenet_library_pch_file})


source_group("include" FILES "${tilenet_include_file}")

# set source groups for the files
foreach(f ${library_source})
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" striped ${f})
	get_filename_component(ending ${f} EXT)

	if(ending STREQUAL ".hpp")
		set(striped "include/${striped}")
	elseif(ending STREQUAL ".cpp")
		set(striped "source/${striped}")
	else()
		message(FATAL_ERROR "'${ending}' is an unknown extension!")
	endif()
	
	get_filename_component(pathed ${striped} PATH)
	string(REPLACE "/" "\\" group_name ${pathed})
	source_group("${group_name}" FILES "${f}")
endforeach(f)




