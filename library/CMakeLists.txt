# macro for adding source
macro(add_library_source files subdirectories)

	# Glob files of this scope
	file(GLOB library_current_source "${files}")

	# add subdirectories
	foreach(dir ${subdirectories})
	#	message("Include: '${dir}'")
		add_subdirectory(${dir})
	endforeach(dir)

	# add source to library_source
	set(library_source "${library_source};${library_current_source}" PARENT_SCOPE)
endmacro(add_library_source)

include_directories("include")

#include subdirectories
add_subdirectory("include")
add_subdirectory("source")

#set all source files for the global scope
set(library_all_source ${library_source} PARENT_SCOPE)


add_library(tilenet ${library_source} )
target_link_libraries(tilenet ${Boost_LIBRARIES})

add_precompiled_header(tilenet	${tilenet_library_pch_file})


# set source groups for the files
foreach(f ${library_source})
	string(REPLACE "${CMAKE_SOURCE_DIR}/library/" "" striped ${f})
	get_filename_component(pathed ${striped} PATH)
	string(REPLACE "/" "\\" group_name ${pathed})
	source_group("${group_name}" FILES "${f}")
endforeach(f)




