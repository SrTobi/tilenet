

file(GLOB library_source "source/*.cpp")
add_library(tilenet SHARED ${tilenet_include_file} ${library_source})

source_group("include" FILES ${tilenet_include_file})
source_group("source" FILES ${library_source})