
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/samples/4con)

file(GLOB sample_four_connect_header *.h)
file(GLOB sample_four_connect_source *.c)

add_executable(4con ${sample_four_connect_source} ${sample_four_connect_header})
target_link_libraries(4con tilenet ${tilenet_dependency_libs})

NEED_MEDIA(4con cour.ttf)
NEED_MEDIA(4con samples/4con)