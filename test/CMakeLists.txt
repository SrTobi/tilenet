

add_executable(ttest test.cpp)
target_link_libraries(ttest tilenet 
	#		${BOOST_FILESYSTEM_LIBRARY}
								${Boost_LIBRARIES}
						)
set_target_properties(ttest PROPERTIES
						LIBRARY_OUTPUT_DIRECTORY bin)

