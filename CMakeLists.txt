cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(EXECUTABLE sc2sim)
set(HEADER_FILES include)
set(CPP_FILES src)

include_directories(${HEADER_FILES})
include_directories(${CPP_FILES})

project(${EXECUTABLE})

# Add all the header files
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

# Add all the source files
set( CPP_FILES
  src/main.cpp
  src/BuildOrder.cpp
  src/BuildStep.cpp
  src/Game.cpp
  src/GameState.cpp
  src/Upgradable.cpp
  src/Updatable.cpp
)

# See: http://stackoverflow.com/q/7031126
#SET (CMAKE_C_COMPILER               "/local/clang-3.5/bin/clang")
SET (CMAKE_C_FLAGS                  "-Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG            "-g")

#SET (CMAKE_CXX_COMPILER             "/local/clang-3.5/bin/clang++")
SET (CMAKE_CXX_FLAGS                "-Wall -std=c++11")
SET (CMAKE_CXX_FLAGS_DEBUG          "-g")

add_executable(${EXECUTABLE} ${CPP_FILES} ${HPP_FILES})
