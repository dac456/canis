cmake_minimum_required(VERSION 2.8)
project(SceneEditor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

add_definitions( -DGLM_FORCE_RADIANS )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost REQUIRED system filesystem)

# Qt/OpenGL
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5X11Extras REQUIRED)
add_definitions(-DQT_NO_KEYWORDS)

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS res/*.qrc)
file(GLOB QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.h)

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles) #little hack to control output of generated Qt files
QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#Assimp - has no find*.cmake, use pkg-config instead
#find_package(PkgConfig)
#pkg_check_modules(PC_ASSIMP QUIET assimp)
#include_directories(${PC_ASSIMP_INCLUDE_DIRS})
#find_library(ASSIMP_LIBRARY NAMES assimp libassimp HINTS ${PC_ASSIMP_LIBDIR} ${PC_ASSIMP_LIBRARY_DIRS})
find_package(Assimp REQUIRED)
find_package(Bullet REQUIRED)

include_directories(${CanisSDK_SOURCE_DIR}/Canis/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/TinyXML)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/SOIL)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIR})
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})

file(GLOB_RECURSE SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)
file(GLOB_RECURSE QT_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS GeneratedFiles/include/*.cpp)

link_directories(${CANIS_BINARY_DIR})
add_executable(SceneEditor ${SOURCE} ${QT_SOURCE} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})

message("${ASSIMP_LIBRARY}")

target_link_libraries(SceneEditor
   ${Qt5Widgets_LIBRARIES}
   ${Qt5OpenGL_LIBRARIES}
   ${OPENGL_LIBRARIES} 
   ${GLEW_LIBRARIES}
   ${GLUT_LIBRARIES}
   ${BULLET_LIBRARIES} 
   ${ASSIMP_LIBRARY}
   Canis
)
