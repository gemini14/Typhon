cmake_minimum_required(VERSION 2.8)

# Project Info
project(Typhon)


#find_package(Boost 1.46)
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS})
#endif()


include_directories(
    ../include/ 
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/Irrlicht/ 
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

${hdr}metrics/metrics.h

${hdr}network/lobbylist.h
${hdr}network/network.h
${hdr}network/networkenet.h
${hdr}network/networkfactory.h
${hdr}network/networklinux.h
${hdr}network/networkraw.h
${hdr}network/networkwin.h

${hdr}utility/username.h
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

metrics/metrics.cpp

network/lobbylist.cpp
network/network.cpp
network/networkenet.cpp
network/networkfactory.cpp
network/networklinux.cpp
network/networkraw.cpp
network/networkwin.cpp

utility/username.cpp
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
)

source_group(Header\ Files\\metrics FILES 
${hdr}metrics/metrics.h
)

source_group(Header\ Files\\network FILES 
${hdr}network/lobbylist.h
${hdr}network/network.h
${hdr}network/networkenet.h
${hdr}network/networkfactory.h
${hdr}network/networklinux.h
${hdr}network/networkraw.h
${hdr}network/networkwin.h
)

source_group(Header\ Files\\utility FILES
${hdr}utility/username.h
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
)

source_group(Source\ Files\\metrics FILES 
metrics/metrics.cpp
)

source_group(Source\ Files\\network FILES 
network/lobbylist.cpp
network/network.cpp
network/networkenet.cpp
network/networkfactory.cpp
network/networklinux.cpp
network/networkraw.cpp
network/networkwin.cpp
)

source_group(Source\ Files\\utility FILES
utility/username.cpp
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../shared/)


if(CMAKE_CL_64)

    message("Creating Win64 project...")
    add_executable(Typhon64 ${SourceFiles} ${HeaderFiles})
    target_link_libraries(Typhon64 debug IrrlichtD_64 optimized Irrlicht_64 Ws2_32 winmm)
    set_target_properties(Typhon64 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_WIN32)

    message("Creating Win32 project")
    add_executable(Typhon32 ${SourceFiles} ${HeaderFiles})
    target_link_libraries(Typhon32 debug IrrlichtD_32 optimized Irrlicht_32 Ws2_32 winmm)
    set_target_properties(Typhon32 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_UNIX)
    
    add_definitions(-g -Wall -std=c++0x -DHAS_SOCKLEN_T)
    
    # Set linux bin directory to a separate folder so that it can be ignored
    # (Linux exe's don't have any extension)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../shared/Linux/)
    
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    
        message("CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P}")
        message("Creating x64 project...")
        #64-bit project (void pointer is 8 bytes/64bits)
        
        add_executable(Typhon64 ${SourceFiles})
        set_target_properties(Typhon64 PROPERTIES DEBUG_POSTFIX _D)
        target_link_libraries(Typhon64 -m64 Irrlicht64.a)
        
    elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    
        message("CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P}")
        message("Creating x86 project...")
        #32-bit project (void pointer is 4 bytes/32 bits)
        
        add_executable(Typhon32 ${SourceFiles})
        set_target_properties(Typhon32 PROPERTIES DEBUG_POSTFIX _D)
        target_link_libraries(Typhon32 -m32 Irrlicht32.a)
    
    endif()

endif()

