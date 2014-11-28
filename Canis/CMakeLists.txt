cmake_minimum_required(VERSION 2.8)
project(Canis)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

add_definitions( -DGLM_FORCE_RADIANS )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost REQUIRED system filesystem)

# Qt/OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

#Assimp - has no find*.cmake, use pkg-config instead
#find_package(PkgConfig)
#pkg_check_modules(PC_ASSIMP QUIET assimp)
#include_directories(${PC_ASSIMP_INCLUDE_DIRS})
#find_library(ASSIMP_LIBRARY NAMES assimp libassimp HINTS ${PC_ASSIMP_LIBDIR} ${PC_ASSIMP_LIBRARY_DIRS})
find_package(Assimp REQUIRED)
find_package(Bullet REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/TinyXML)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SOIL)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})

file(GLOB_RECURSE CS_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp src/SOIL/*.c src/TinyXML/*.cpp)

add_library(Canis SHARED ${CS_SOURCE})

target_link_libraries(Canis
   ${Boost_FILESYSTEM_LIBRARY}
   ${OPENGL_LIBRARIES} 
   ${GLEW_LIBRARIES}
   ${GLUT_LIBRARIES}
   ${BULLET_LIBRARIES} 
   ${ASSIMP_LIBRARY}
)
