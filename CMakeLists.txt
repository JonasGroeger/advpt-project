cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

set( EXECUTABLE sc2sim )
set( HEADER_FOLDER include )
set( CPP_FOLDER src )

include_directories( ${HEADER_FOLDER} )
include_directories( ${CPP_FOLDER} )
project( ${EXECUTABLE} )

set( CMAKE_BUILD_TYPE Debug )
# See: http://stackoverflow.com/q/7031126
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  set( CMAKE_C_FLAGS                  "-Wall -std=c11" )
  set( CMAKE_C_FLAGS_DEBUG            "-g" )
  set( CMAKE_CXX_FLAGS                "-Wall -std=c++11" )
  set( CMAKE_CXX_FLAGS_DEBUG          "-g" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  set( CMAKE_C_FLAGS                  "-Wall -std=c11" )
  set( CMAKE_C_FLAGS_DEBUG            "-g" )
  set( CMAKE_CXX_FLAGS                "-Wall -std=c++11" )
  set( CMAKE_CXX_FLAGS_DEBUG          "-g" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
  # using Intel C++
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # Turn on the ability to create folders to organize projects (.vcxproj)
  # It creates "CMakePredefinedTargets" folder by default and adds CMake
  # defined projects like INSTALL.vcxproj and ZERO_CHECK.vcxproj
  set_property( GLOBAL PROPERTY USE_FOLDERS ON )
endif()

# Output some properties for easy debugging
message( "C COMPILER   -->" ${CMAKE_C_COMPILER} )
message( "CXX COMPILER -->" ${CMAKE_CXX_COMPILER} )
message( "C FLAGS:     -->" ${CMAKE_C_FLAGS} )
message( "CXX FLAGS:   -->" ${CMAKE_CXX_FLAGS} )

# Add all the header files ...
set( HPP_FILES
  include/BuildOrder.hpp
  include/BuildStep.hpp
  include/Constructable.hpp
  include/Entity.hpp
  include/EntityType.hpp
  include/Game.hpp
  include/GameState.hpp
  include/Producer.hpp
  include/Updatable.hpp
  include/Upgradable.hpp
)

# ... and source files ...
set( CPP_FILES
  src/BuildOrder.cpp
  src/BuildStep.cpp
  src/Game.cpp
  src/GameState.cpp
  src/main.cpp
  src/Updatable.cpp
  src/Upgradable.cpp
)

source_group( "src" FILES ${CPP_FILES} )
source_group( "include" FILES ${HPP_FILES} )

# ... to build the executable.
add_executable( ${EXECUTABLE} ${CPP_FILES} ${HPP_FILES} )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # Creates a folder "executables" and adds target project (sc2sim.vcxproj) under it
  set_property( TARGET ${EXECUTABLE} PROPERTY FOLDER "executables" )
endif()
