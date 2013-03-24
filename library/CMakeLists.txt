

file(GLOB library_client_include "include/client/*.hpp")
file(GLOB library_client_source "source/client/*.cpp")

file(GLOB library_server_include "include/server/*.hpp")
file(GLOB library_server_source "source/server/*.cpp")

file(GLOB library_server_acceptors_include "include/server/acceptors/*.hpp")
file(GLOB library_server_acceptors_source "source/server/acceptors/*.cpp")

file(GLOB library_utils_include "include/utils/*.hpp")
file(GLOB library_utils_source "source/utils/*.cpp")

file(GLOB library_include "include/*.hpp")
file(GLOB library_source "source/*.cpp")

set(library_all_source 	${tilenet_include_file}
						${library_source} ${library_include}
						${library_client_source} ${library_client_include}
						${library_server_acceptors_include} ${library_server_acceptors_source}
						${library_server_source} ${library_server_include}
						${library_utils_source} ${library_utils_include})
						
set(library_all_source ${library_all_source} PARENT_SCOPE)

include_directories(include)

add_library(tilenet ${library_all_source} )
target_link_libraries(tilenet ${Boost_LIBRARIES})

add_precompiled_header(tilenet	${tilenet_library_pch_file})

source_group("include" FILES ${tilenet_include_file})
source_group("include" FILES ${library_include})
source_group("source" FILES ${library_source})

source_group("include\\client" FILES ${library_client_include})
source_group("source\\client" FILES ${library_client_source})

source_group("include\\server" FILES ${library_server_include})
source_group("source\\server" FILES ${library_server_source})

source_group("include\\server\\acceptors" FILES ${library_server_acceptors_include})
source_group("source\\server\\acceptors" FILES ${library_server_acceptors_source})

source_group("include\\utils" FILES ${library_utils_include})
source_group("source\\utils" FILES ${library_utils_source})
