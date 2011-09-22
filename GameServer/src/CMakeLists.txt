cmake_minimum_required(VERSION 2.8)

#Project Info
project(TyphonServer)

include_directories(
	../include/
)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/luabind/
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/lua/
)


set(hdr ../include/)


set(HeaderFiles

${hdr}enet/callbacks.h
${hdr}enet/enet.h
${hdr}enet/list.h
${hdr}enet/protocol.h
${hdr}enet/time.h
${hdr}enet/types.h
${hdr}enet/unix.h
${hdr}enet/utility.h
${hdr}enet/win32.h

${hdr}logger/logger.h

${hdr}network/network.h
${hdr}network/networkenetserver.h
${hdr}network/networkfactory.h

${hdr}server/server.h

${hdr}utility/constants.h
${hdr}utility/utility.h
)


set(SourceFiles

main.cpp

enet/callbacks.c
enet/compress.c
enet/host.c
enet/list.c
enet/packet.c
enet/peer.c
enet/protocol.c
enet/unix.c
enet/win32.c

logger/logger.cpp

network/network.cpp
network/networkenetserver.cpp
network/networkfactory.cpp

server/server.cpp

utility/utility.cpp
)


set(ScriptFiles
)


source_group(Header\ Files\\enet FILES
${hdr}enet/callbacks.h
${hdr}enet/enet.h
${hdr}enet/list.h
${hdr}enet/protocol.h
${hdr}enet/time.h
${hdr}enet/types.h
${hdr}enet/unix.h
${hdr}enet/utility.h
${hdr}enet/win32.h
)

source_group(Header\ Files\\logger FILES
${hdr}logger/logger.h
)

source_group(Header\ Files\\network FILES
${hdr}network/network.h
${hdr}network/networkenetserver.h
${hdr}network/networkfactory.h
)

source_group(Header\ Files\\server FILES
${hdr}server/server.h
)

source_group(Header\ Files\\utility FILES
${hdr}utility/constants.h
${hdr}utility/utility.h
)


source_group(Source\ Files\\enet FILES
enet/callbacks.c
enet/compress.c
enet/host.c
enet/list.c
enet/packet.c
enet/peer.c
enet/protocol.c
enet/unix.c
enet/win32.c
)

source_group(Source\ Files\\logger FILES
logger/logger.cpp
)

source_group(Source\ Files\\network FILES
network/network.cpp
network/networkenetserver.cpp
network/networkfactory.cpp
)

source_group(Source\ Files\\server FILES
server/server.cpp
)

source_group(Source\ Files\\utility FILES
utility/utility.cpp
)


if(CMAKE_CL_64)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Server/Win64/)
	message("Creating Win64 Server project...")
	add_executable(TyphonServer64 ${SourceFiles} ${HeaderFiles} ${ScriptFiles})
	target_link_libraries(TyphonServer64
		lua5.1_64
		debug luabindD_64 optimized luabind_64
		Ws2_32
		winmm
		debug boost_thread-vc100-mt-gd-1_47 optimized boost_thread-vc100-mt-1_47)
	set_target_properties(TyphonServer64 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_WIN32)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Server/Win32/)
	message("Creating Win32 Server project...")
	add_executable(TyphonServer32 ${SourceFiles} ${HeaderFiles} ${ScriptFiles})
	target_link_libraries(TyphonServer32
		lua5.1_32
		debug luabindD_32 optimized luabind_32
		Ws2_32
		winmm
		debug boost_thread-vc100-mt-gd-1_47 optimized boost_thread-vc100-mt-1_47)
	set_target_properties(TyphonServer32 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_UNIX)

	add_definitions(-g -Wall -std=c++0x -DHAS_SOCKLEN_T)

	# Set Linux bin directory to a separate folder so that it can be ignored
	# (Linux exe's don't have any extension)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Server/Linux/)

	message("Creating Server project...")

	add_executable(TyphonServer ${SourceFiles})
	set_target_properties(TyphonServer PROPERTIES DEBUG_POSTFIX _D)
	target_link_libraries(TyphonServer lua5.1 luabind z)

endif()
