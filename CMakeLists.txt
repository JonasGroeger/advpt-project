cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(EXECUTABLE sc2sim)
set(HEADER_FILES include)
set(CPP_FILES src)

include_directories(${HEADER_FILES})
include_directories(${CPP})

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
)

add_executable(${EXECUTABLE} ${CPP_FILES} ${HPP_FILES})
