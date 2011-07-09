cmake_minimum_required(VERSION 2.8)

# Project Info
project(Typhon)


include_directories(
    ../include/ 
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/ 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/Irrlicht/ 
)

set(Hdr ../include/)

set(HeaderFiles
${Hdr}metrics/metrics.h
)

set(SourceFiles 
main.cpp 
metrics/metrics.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../shared/)


if(CMAKE_CL_64)

    message("Creating Win64 project...")
    add_executable(Typhon64 ${SourceFiles} ${HeaderFiles})
    target_link_libraries(Typhon64 debug IrrlichtD_64 optimized Irrlicht_64)
    set_target_properties(Typhon64 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_WIN32)

    message("Creating Win32 project")
    add_executable(Typhon32 ${SourceFiles} ${HeaderFiles})
    target_link_libraries(Typhon32 debug IrrlichtD_32 optimized Irrlicht_32)
    set_target_properties(Typhon32 PROPERTIES PREFIX "../")

elseif(CMAKE_HOST_UNIX)
    
    add_definitions(-g -Wall -std=c++0x)
    
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

