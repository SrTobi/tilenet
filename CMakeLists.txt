cmake_minimum_required( VERSION 2.8 )

project(tilenet)

################### set module path ###################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/extras/cmake/Modules/")


################### setup options ###################
set(Option_DEFAULT_MAKE_DOXYGEN_TARGET ON)

if(MSVC)
set(Boost_USE_STATIC_LIBS ON)
set(Option_DEFAULT_MAKE_DOXYGEN_TARGET OFF)
endif(MSVC)


option(Tilenet_USE_PRE_COMPILED_HEADER		"Use precompiled header for compilation" ON)
option(Tilenet_BUILD_SHARED_LIB				"Build tilenet as shared library" OFF)
option(Tilenet_MAKE_DOXYGEN_TARGET			"Create a doxygen build target" ${Option_DEFAULT_MAKE_DOXYGEN_TARGET})
option(Tilenet_RETHROW_THREAD_EXCEPTIONS	"Rethrow exceptions in worker threads, instead of catch them silently" ON)
option(Tilenet_COPY_MEDIA					"Copies the media files for samples and tests into the target directories" ON)


################### setup pre compiled header macro ###################
if(Tilenet_USE_PRE_COMPILED_HEADER)
	include("extras/cmake/pch_for_cmake.txt")
	add_definitions(-DTILENET_USE_PRECOMPILED_HEADER)
else(Tilenet_USE_PRE_COMPILED_HEADER)

	# simply set an empty macro
	macro(ADD_PRECOMPILED_HEADER _targetName _input)
	endmacro()

endif(Tilenet_USE_PRE_COMPILED_HEADER)

			
################### add c++11 support ###################
if(NOT MSVC)
	add_definitions("-std=c++0x")
endif(NOT MSVC)


################### check std::unordered_map::emplace ###################

if(DEFINED Tilenet_USE_ASSOZIATIVE_EMPLACE)
	set(has_emplace ON)
else()
	try_compile(has_emplace
				${PROJECT_SOURCE_DIR}/extras/cmake/
				${PROJECT_SOURCE_DIR}/extras/cmake/check_emplace.cpp)

endif()


option(Tilenet_USE_ASSOZIATIVE_EMPLACE	"Copies the media files for samples and tests into the target directories" ${has_emplace})

if(${Tilenet_USE_ASSOZIATIVE_EMPLACE})
	add_definitions("-DTILENET_USE_ASSOZIATIVE_EMPLACE")
endif()


################### set definitions ###################
if(Tilenet_RETHROW_THREAD_EXCEPTIONS)
	add_definitions(-DTILENET_RETHROW_THREAD_EXCEPTIONS)
endif(Tilenet_RETHROW_THREAD_EXCEPTIONS)


if(NOT Boost_USE_STATIC_LIBS)
	add_definitions(-DBOOST_TEST_DYN_LINK) 
endif(NOT Boost_USE_STATIC_LIBS)
add_definitions(-DTILENET_AS_LIB)
add_definitions( -DBOOST_ALL_NO_LIB )


################### find packages ###################
find_package(Boost COMPONENTS system thread serialization regex unit_test_framework filesystem REQUIRED)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)


################### setup target directories ###################
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)



################### add c++11 support ###################
set(rxml_includes				"${PROJECT_SOURCE_DIR}/extras/rxml/include")
set(rapidxml_includes			"${PROJECT_SOURCE_DIR}/extras/rapidxml-1.13")
set(tilenet_library_pch_file	"${PROJECT_SOURCE_DIR}/library/includes.hpp")
set(tilenet_include_file		"${PROJECT_SOURCE_DIR}/include/tilenet.h")
set(tilenet_debug_include_file	"${PROJECT_SOURCE_DIR}/include/tilenet_debug.h")


################### setup include directories ###################
include_directories("include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${rapidxml_includes})
include_directories(${rxml_includes})


################### setup link directories ###################
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${BOOST_LIBRARYDIR})

################### set dependencies for tilenet-lib ###################
set(tilenet_dependency_libs ${Boost_LIBRARIES} ${SFML_LIBRARIES})


################### setup media copy support ###################

function(NEED_MEDIA _target _resource)
	
	if(Tilenet_COPY_MEDIA)
		set(res "${PROJECT_SOURCE_DIR}/media/${_resource}")
		
		if(NOT EXISTS ${res})
			message(SEND_ERROR "[${_resource}] does not exist in media!")
		endif()
		
		if(IS_DIRECTORY ${res})
			set(copy_command  copy_directory)
		else()
			set(copy_command copy_if_different)
		endif()

		add_custom_command(	TARGET ${_target} POST_BUILD
							COMMAND ${CMAKE_COMMAND} -E ${copy_command}
							${res}
							$<TARGET_FILE_DIR:${_target}>)
	endif(Tilenet_COPY_MEDIA)
endfunction()

################### add directories with source###################
add_subdirectory(library)
add_subdirectory(samples)
add_subdirectory(tests)



################### setup documentation target ###################
# add a target to generate API documentation with Doxygen
if(Tilenet_MAKE_DOXYGEN_TARGET)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc.doxy ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy @ONLY)
		add_custom_target(documentation ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc.doxy
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	endif(DOXYGEN_FOUND)
endif(Tilenet_MAKE_DOXYGEN_TARGET)
