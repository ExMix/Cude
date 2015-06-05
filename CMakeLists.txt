cmake_minimum_required(VERSION 3.1)
project(Cude)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(QUARTZ QuartzCore)
FIND_LIBRARY(IO_KIT IOKit)
FIND_LIBRARY(OGL OpenGL)

set(BASE_FOLDER ${CMAKE_CURRENT_LIST_DIR}/base)
set(OGL_FOLDER ${CMAKE_CURRENT_LIST_DIR}/ogl)
set(RESOURCE_FOLDER ${CMAKE_CURRENT_LIST_DIR}/resources)

set(RESOURCE_PATH "../Resources/")

add_subdirectory(${BASE_FOLDER})
add_subdirectory(${OGL_FOLDER})
add_subdirectory(${RESOURCE_FOLDER})

file(GLOB BASE_SOURCE ${BASE_FOLDER}/*.cpp ${BASE_FOLDER}/*.hpp)
file(GLOB OGL_SOURCE ${OGL_FOLDER}/*.cpp ${BASE_FOLDER}/*.hpp)
file(GLOB SOURCES *.cpp *.hpp)
file(GLOB RESOURCE_FILES ${RESOURCE_FOLDER}/*.vsh
                         ${RESOURCE_FOLDER}/*.fsh
                         ${RESOURCE_FOLDER}/*.png)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../glm)

configure_file(${BASE_FOLDER}/path_config.hpp.in ${BASE_FOLDER}/path_config.hpp @ONLY)
add_executable(Cude MACOSX_BUNDLE ${SOURCES} ${OGL_SOURCE} ${BASE_SOURCE} ${RESOURCE_FILES})

set_source_files_properties(${RESOURCE_FILES}
                            PROPERTIES
                            MACOSX_PACKAGE_LOCATION Resources)
target_link_libraries(Cude libglfw3.a ${COCOA_LIBRARY} ${QUARTZ} ${IO_KIT} ${OGL})
