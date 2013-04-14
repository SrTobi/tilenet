



add_library_source("*.cpp" "client;network;server;utils")


#set(group			"source")
#set(subdirectories	"client network server utils")


########################################################################
# Set source group for this scope
#set(library_current_group "${group}\\source")

# Files of this scope
#file(GLOB library_current_source "*.cpp")

# set group for files of this scope
#source_group(${group} FILES ${source})

# subdirectories
#foreach(dir ${subdirectories})
#	add_subdirectory(${dir})
#endforeach(dir)

#set(library_source "${library_source} ${source}" PARENT_SCOPE)