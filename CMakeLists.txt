cmake_minimum_required(VERSION 2.8)

option(INTEL_CC "Use the Intel compiler." OFF)
option(DIST_CC "Use distcc." OFF)

if (INTEL_CC)
    find_program(CMAKE_C_COMPILER NAMES icc)
    find_program(CMAKE_CXX_COMPILER NAMES icpc)
    find_program(CMAKE_AR NAMES xiar)
    find_program(CMAKE_LINKER NAMES xild)
    
    set(CMAKE_CXX_FLAGS "-O0 -g -traceback")
    #set(CMAKE_CXX_FLAGS "-O3")

    if (CMAKE_C_COMPILER MATCHES CMAKE_C_COMPILER-NOTFOUND OR
     CMAKE_CXX_COMPILER MATCHES CMAKE_CXX_COMPILER-NOTFOUND OR
     CMAKE_AR MATCHES CMAKE_AR-NOTFOUND OR
     CMAKE_LINKER MATCHES CMAKE_LINKER-NOTFOUND)
        message(FATAL_ERROR "Cannot find Intel compiler.  You may need to run `. /opt/intel/bin/compilervars.sh intel64'")
    endif ()
endif (INTEL_CC)

if(DIST_CC)
    set(CMAKE_CXX_COMPILER "distcc g++")
    set(CMAKE_C_COMPILER "distcc gcc")
endif(DIST_CC)

message("USING CXX COMPILER: ${CMAKE_CXX_COMPILER}")
message("USING CXX FLAGS: ${CMAKE_CXX_FLAGS}")

project(CanisSDK)

add_subdirectory(Canis)
add_subdirectory(SceneEditor)
