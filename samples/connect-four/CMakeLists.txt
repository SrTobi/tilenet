
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/samples/con4)

file(GLOB sample_connect_four_header *.h)
file(GLOB sample_connect_four_source *.c)

add_executable(con4 ${sample_connect_four_source} ${sample_connect_four_header})
target_link_libraries(con4 tilenet ${tilenet_dependency_libs})

NEED_MEDIA(con4 cour.ttf)
NEED_MEDIA(con4 samples/con4)