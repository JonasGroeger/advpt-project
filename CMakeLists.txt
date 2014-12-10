cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(sc2sim)

# Add all the header files
set( HPP_FILES
  include/BuildOrder.hpp
  include/BuildStep.hpp
  include/Constructable.hpp
  include/Decimal.hpp
  include/Entitiy.hpp
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
)

add_executable( sc2sim ${CPP_FILES} ${HPP_FILES} )
