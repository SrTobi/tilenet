cmake_minimum_required( VERSION 2.8 )

project(tilenet)


#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(Option_DEFAULT_MAKE_DOXYGEN_TARGET ON)

if(MSVC)
set(Boost_USE_STATIC_LIBS ON)
set(Option_DEFAULT_MAKE_DOXYGEN_TARGET OFF)
endif(MSVC)


option(Tilenet_USE_PRE_COMPILED_HEADER		"Use precompiled header for compilation" ON)
option(Tilenet_BUILD_SHARED_LIB				"Build tilenet as shared library" OFF)
option(Tilenet_MAKE_DOXYGEN_TARGET			"Create a doxygen build target" ${Option_DEFAULT_MAKE_DOXYGEN_TARGET})
option(Tilenet_RETHROW_THREAD_EXCEPTIONS	"Rethrow exceptions in worker threads, instead of catch them silently" ON)

if(Tilenet_USE_PRE_COMPILED_HEADER)
	add_subdirectory("extras/pch")
	add_definitions(-DTILENET_USE_PRECOMPILED_HEADER)
else(Tilenet_USE_PRE_COMPILED_HEADER)

	# simply set an empty macro
	MACRO(ADD_PRECOMPILED_HEADER _targetName _input)
	ENDMACRO()

endif(Tilenet_USE_PRE_COMPILED_HEADER)

if(Tilenet_RETHROW_THREAD_EXCEPTIONS)
	add_definitions(-DTILENET_RETHROW_THREAD_EXCEPTIONS)
endif(Tilenet_RETHROW_THREAD_EXCEPTIONS)

if(NOT MSVC)
	ADD_DEFINITIONS("-std=c++0x")
endif(NOT MSVC)

if(NOT Boost_USE_STATIC_LIBS)
	add_definitions(-DBOOST_TEST_DYN_LINK) 
endif(NOT Boost_USE_STATIC_LIBS)
add_definitions(-DTILENET_AS_LIB)
add_definitions( -DBOOST_ALL_NO_LIB )

# find packages
find_package(Boost COMPONENTS system thread serialization unit_test_framework REQUIRED)
find_package(SFML COMPONENTS system window graphics audio REQUIRED)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(tilenet_library_pch_file	"${PROJECT_SOURCE_DIR}/library/includes.hpp")
set(tilenet_include_file		"${PROJECT_SOURCE_DIR}/include/tilenet.h")


include_directories(include ${Boost_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${BOOST_LIBRARYDIR})

set(tilenet_dependency_libs ${Boost_LIBRARIES} ${SFML_LIBRARIES})


add_subdirectory(library)
add_subdirectory(samples)
add_subdirectory(tests)


#add_executable(sync4cpp		 		${include_files} ${sources_files})
#add_executable(sync4cpp-tests 		${include_files} ${tests_files})

#target_link_libraries(sync4cpp-tests 		${Boost_LIBRARIES})


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
