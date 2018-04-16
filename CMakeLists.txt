# I don't really know which version of cmake is required
cmake_minimum_required (VERSION 2.6)
project (Heimo)

#from the OGRE tutorials
find_package(OGRE 1.10 REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
link_directories(${OGRE_LIBRARY_DIRS})

#get the proper configuration files
file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})

#this tells the linker where the dynamic libraries are
link_libraries(${OGRE_LIBRARIES})

#set any optional compilation flags
SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11 -Wall -v -D DEBUG")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(Heimo Heimo.cpp)
