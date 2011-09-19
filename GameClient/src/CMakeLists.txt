cmake_minimum_required(VERSION 2.8)

# Project Info
project(TyphonClient)


include_directories(
	../include/
	../include/freetype/ 
)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/ 
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/freetype 
	${CMAKE_CURRENT_SOURCE_DIR}/../lib/Irrlicht/ 
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

${hdr}engine/engine.h
${hdr}engine/luamanager.h

${hdr}font/font_manager.h
${hdr}font/gui_freetype_font.h

${hdr}i18n/i18n.h

${hdr}logger/logger.h

${hdr}metrics/metrics.h

${hdr}network/lobbylist.h
${hdr}network/network.h
${hdr}network/networkenetclient.h
${hdr}network/networkfactory.h
${hdr}network/networklinux.h
${hdr}network/networkraw.h
${hdr}network/networkwin.h

${hdr}player/lobbyplayer.h

${hdr}state/fsmevents.h
${hdr}state/fsmstate.h
${hdr}state/game.h
${hdr}state/lobby.h
${hdr}state/machine.h
${hdr}state/mainmenu.h
${hdr}state/options.h

${hdr}utility/constants.h
${hdr}utility/stateexception.h
${hdr}utility/useroptions.h
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

engine/engine.cpp
engine/luamanager.cpp

font/font_manager.cpp
font/gui_freetype_font.cpp

i18n/i18n.cpp

logger/logger.cpp

metrics/metrics.cpp

network/lobbylist.cpp
network/network.cpp
network/networkenetclient.cpp
network/networkfactory.cpp
network/networklinux.cpp
network/networkraw.cpp
network/networkwin.cpp

player/lobbyplayer.cpp

state/game.cpp
state/lobby.cpp
state/mainmenu.cpp
state/options.cpp

utility/utility.cpp
)


set(ScriptFiles
scripts/de_lang.lua
scripts/en_lang.lua
scripts/input_codes.lua
scripts/lang_get.lua
scripts/script_list.lua
scripts/user_data.lua
scripts/user_options.lua
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

source_group(Header\ Files\\engine FILES
${hdr}engine/engine.h
${hdr}engine/luamanager.h
)

source_group(Header\ Files\\font FILES
${hdr}font/font_manager.h
${hdr}font/gui_freetype_font.h
)

source_group(Header\ Files\\i18n FILES
${hdr}i18n/i18n.h
)

source_group(Header\ Files\\logger FILES
${hdr}logger/logger.h
)

source_group(Header\ Files\\metrics FILES 
${hdr}metrics/metrics.h
)

source_group(Header\ Files\\network FILES 
${hdr}network/lobbylist.h
${hdr}network/network.h
${hdr}network/networkenetclient.h
${hdr}network/networkfactory.h
${hdr}network/networklinux.h
${hdr}network/networkraw.h
${hdr}network/networkwin.h
)

source_group(Header\ Files\\player FILES
${hdr}player/lobbyplayer.h
)

source_group(Header\ Files\\state FILES
${hdr}state/fsmevents.h
${hdr}state/fsmstate.h
${hdr}state/game.h
${hdr}state/lobby.h
${hdr}state/machine.h
${hdr}state/mainmenu.h
${hdr}state/options.h
)

source_group(Header\ Files\\utility FILES
${hdr}utility/constants.h
${hdr}utility/stateexception.h
${hdr}utility/useroptions.h
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

source_group(Source\ Files\\engine FILES
engine/engine.cpp
engine/luamanager.cpp
)

source_group(Source\ Files\\font FILES
font/font_manager.cpp
font/gui_freetype_font.cpp
)

source_group(Source\ Files\\i18n FILES
i18n/i18n.cpp
)

source_group(Source\ Files\\logger FILES
logger/logger.cpp
)

source_group(Source\ Files\\metrics FILES 
metrics/metrics.cpp
)

source_group(Source\ Files\\network FILES 
network/lobbylist.cpp
network/network.cpp
network/networkenetclient.cpp
network/networkfactory.cpp
network/networklinux.cpp
network/networkraw.cpp
network/networkwin.cpp
)

source_group(Source\ Files\\player FILES
player/lobbyplayer.cpp
)

source_group(Source\ Files\\scripts FILES 
${ScriptFiles}
)

source_group(Source\ Files\\state FILES
state/game.cpp
state/lobby.cpp
state/mainmenu.cpp
state/options.cpp
)

source_group(Source\ Files\\utility FILES
utility/utility.cpp
)


if(CMAKE_CL_64)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Client/Win64/)
	message("Creating Win64 project...")
	add_executable(TyphonClient64 ${SourceFiles} ${HeaderFiles} ${ScriptFiles})
	target_link_libraries(TyphonClient64 
		debug IrrlichtD_64 optimized Irrlicht_64 
		lua5.1_64 
		debug luabindD_64 optimized luabind_64 
		Ws2_32 
		winmm
		debug freetype246D_64 optimized freetype246_64
		debug boost_thread-vc100-mt-gd-1_47 optimized boost_thread-vc100-mt-1_47)
	set_target_properties(TyphonClient64 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_WIN32)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Client/Win32/)
	message("Creating Win32 project")
	add_executable(TyphonClient32 ${SourceFiles} ${HeaderFiles} ${ScriptFiles})
	target_link_libraries(TyphonClient32 
		debug IrrlichtD_32 optimized Irrlicht_32
		lua5.1_32 
		debug luabindD_32 optimized luabind_32 
		Ws2_32 
		winmm
		debug freetype246D_32 optimized freetype246_32
		debug boost_thread-vc100-mt-gd-1_47 optimized boost_thread-vc100-mt-1_47)
	set_target_properties(TyphonClient32 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_UNIX)
	
	add_definitions(-g -Wall -std=c++0x -DHAS_SOCKLEN_T)
	
	# Set linux bin directory to a separate folder so that it can be ignored
	# (Linux exe's don't have any extension)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Client/Linux/)
	
	message("Creating Client project...")
		   
	add_executable(TyphonClient ${SourceFiles})
	set_target_properties(TyphonClient PROPERTIES DEBUG_POSTFIX _D)
	target_link_libraries(TyphonClient Irrlicht lua5.1 luabind freetype z boost_thread-mt pthread)

endif()

