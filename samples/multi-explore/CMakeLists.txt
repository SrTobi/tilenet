
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/samples/multi-explore)

file(GLOB sample_multi_explore_header *.hpp)
file(GLOB sample_multi_explore_source *.cpp)

add_executable(multi-explore ${sample_multi_explore_header} ${sample_multi_explore_source})
target_link_libraries(multi-explore tilenet ${tilenet_dependency_libs})

NEED_MEDIA(multi-explore cour.ttf)
NEED_MEDIA(multi-explore samples/multi-explore)