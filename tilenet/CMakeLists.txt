


include_directories("include")

set(tilenet_src "src/tilenet.cpp"
				"src/tiley/tiley.cpp"

				"src/server/server.cpp"
				"src/server/display.cpp"
				"src/server/layer.cpp"

				"src/client/client.cpp"
				"src/client/messenger.cpp"
#				"src/client/clientLayer.cpp"
#				"src/client/net_client.cpp"

				"src/client/v1/clientLayer.cpp"
				"src/client/v1/clientLogic.cpp"


				"src/network/network.cpp"
#				"src/network/netmgr.cpp"
				"src/network/buffer_base.cpp"
				"src/network/ibuffer.cpp"
				"src/network/obuffer.cpp"
				"src/network/netreceiver.cpp"

				"src/service/netservice.cpp"
				"src/service/participant.cpp"
				"src/service/pending.cpp"

				)
				#"src/random.cpp")

set(SFML_LIBS_DEBUG
		${SFML_SYSTEM_LIBRARY_DEBUG}
		${SFML_WINDOW_LIBRARY_DEBUG}
		${SFML_GRAPHICS_LIBRARY_DEBUG}
		${SFML_NETWORK_LIBRARY_DEBUG})


add_library(tilenet SHARED ${tilenet_src})
target_link_libraries(tilenet 
						${SFML_LIBS_DEBUG}
						#${Boost_LIBRARIES}
						)


#set_target_properties(tilenet PROPERTIES
#						LIBRARY_OUTPUT_DIRECTORY lib)
