# FileName: CMakeLists.txt
# Description: Configures, makes and installs MPIMIN.
# Copyright (C) 2014  K M Masum Habib <masum.habib@gmail.com>
# Created on: Nov 02, 2014

cmake_minimum_required(VERSION 2.8)

# Version
set(MPIMIN_MAJOR 0)
set(MPIMIN_MINOR 00)
set(MPIMIN_PATCH 1)

set(MPIMIN_CXX_11 on)
set(CMAKE_VERBOSE_MAKEFILE      off)
#set(LINK_SEARCH_START_STATIC    on)
#set(CMAKE_BUILD_TYPE "Debug")

# Start configuration
set(MPIMIN_VERSION ${MPIMIN_MAJOR}.${MPIMIN_MINOR}.${MPIMIN_PATCH})
message(STATUS "Configuring MPIMIN ${MPIMIN_VERSION}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

project(mpimin CXX)

# For custom cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Options
# -----------------------------------------------------------------------------

#Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" )
endif()

# Compiler flags
if (MPIMIN_CXX_11)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# optimization
#set(CMAKE_CXX_FLAGS_RELEASE " -m64 -O2 -MMD -MP -MF ")

# Libraries
# -----------------------------------------------------------------------------
# MPI
find_package(MPI REQUIRED)

# Boost
# Shared linking of libboost_python is required for mpi::communicator
# to work both in python and C++.
set(Boost_USE_MULTITHREADED     off)
if(LINK_STATIC)
    set(Boost_USE_STATIC_LIBS       off)
endif()
find_package(Boost 1.55 REQUIRED python)
set(Boost_PYTHON_LIBRARIES_SHARED ${Boost_PYTHON_LIBRARIES})
unset(Boost_LIBRARIES)
unset(Boost_PYTHON_LIBRARIES)
if(LINK_STATIC)
    set(Boost_USE_STATIC_LIBS       on)
endif()
find_package(Boost 1.55 REQUIRED serialization mpi)

# Sources and headers
# -----------------------------------------------------------------------------
# Prepare distribution directory
set(MPIMIN_DIST_DIR ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-${MPIMIN_VERSION})
set(EXECUTABLE_OUTPUT_PATH ${MPIMIN_DIST_DIR}/bin)

# version header
configure_file (
    "${CMAKE_SOURCE_DIR}/include/config.in.h"
    "${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/include/config.h"
)

# includes
set(MPIMIN_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
list(REMOVE_DUPLICATES MPIMIN_INCLUDE_DIRS)
list(APPEND MPIMIN_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/include)

include_directories(${MPIMIN_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${MPI_INCLUDE_PATH})

# -------- <MPIMIN C++ Executable> ------------------------------------------------
# sources
file(GLOB_RECURSE MPIMIN_SOURCES "src/*.cpp")
file(GLOB_RECURSE MPIMIN_HEADERS "include/*.h*")
add_executable(mpimin ${MPIMIN_SOURCES} ${MPIMIN_HEADERS})


# linking
target_link_libraries (mpimin ${Boost_MPI_LIBRARIES})
target_link_libraries (mpimin ${Boost_SERIALIZATION_LIBRARIES})

# Prepare mpimin package
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND cp -rp ${MPIMIN_DIST_DIR}/bin/mpimin ${CMAKE_SOURCE_DIR}/bin
)

# -------- </MPIMIN C++ Executable> -----------------------------------------------

# installation
install(DIRECTORY ${MPIMIN_DIST_DIR} DESTINATION .)

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(MPIMIN_FLAGS ${CMAKE_CXX_FLAGS_RELEASE})
endif(${CMAKE_BUILD_TYPE} MATCHES "Release")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(MPIMIN_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
endif(${CMAKE_BUILD_TYPE} MATCHES "Debug")

message(STATUS "Compiler flags: ${MPIMIN_FLAGS}")
message(STATUS "Linker flags: ${CMAKE_SHARED_LINKER_FLAGS}")

message(STATUS "Installation location: ${CMAKE_INSTALL_PREFIX}/${MPIMIN_INSTALL_DIR}")

