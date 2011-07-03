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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../shared/)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")
endif()


if(CMAKE_CL_64)
    add_executable(Typhon64 main.cpp)
    target_link_libraries(Typhon64 debug IrrlichtD_64 optimized Irrlicht_64)
    set_target_properties(Typhon64 PROPERTIES PREFIX "../")
elseif(CMAKE_HOST_WIN32)
    add_executable(Typhon32 main.cpp)
    target_link_libraries(Typhon32 debug IrrlichtD_32 optimized Irrlicht_32)
    set_target_properties(Typhon32 PROPERTIES PREFIX "../")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(Typhon irrlicht.a)
endif()

