cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

set( EXECUTABLE sc2sim )
set( HEADER_FOLDER include )
set( CPP_FOLDER src )

include_directories( ${HEADER_FOLDER} )
include_directories( ${CPP_FOLDER} )

set( CMAKE_BUILD_TYPE Debug )
# See: http://stackoverflow.com/q/7031126
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set( CMAKE_C_FLAGS                  "-Wall -Werror -std=c11" )
    set( CMAKE_C_FLAGS_DEBUG            "-g" )
    set( CMAKE_CXX_FLAGS                "-Wall -Werror -std=c++11" )
    set( CMAKE_CXX_FLAGS_DEBUG          "-g" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    set( CMAKE_C_FLAGS                  "-Wall -Werror -std=c11" )
    set( CMAKE_C_FLAGS_DEBUG            "-g" )
    set( CMAKE_CXX_FLAGS                "-Wall -Werror -std=c++11" )
    set( CMAKE_CXX_FLAGS_DEBUG          "-g" )
endif()

# Output some properties for easy debugging
message( "C COMPILER   -> " ${CMAKE_C_COMPILER} )
message( "CXX COMPILER -> " ${CMAKE_CXX_COMPILER} )
message( "C FLAGS:     -> " ${CMAKE_C_FLAGS} )
message( "CXX FLAGS:   -> " ${CMAKE_CXX_FLAGS} )

# Add all the header files ...
set( HPP_FILES
    include/BuildAction.h
    include/BuildOrder.h
    include/ConfigParser.h
    include/Config.h
    include/INIReader.h
    include/ini.h
    include/State.h
    include/Strategy.h
    include/GeneticOptimizer.h
    include/tinyxml2.h
)

# ... and source files ...
set( CPP_FILES
  src/BuildOrder.cpp
  src/ConfigParser.cpp
  src/Config.cpp
  src/INIReader.cpp
  src/ini.c
  src/GeneticOptimizer.cpp
  src/State.cpp
  src/tinyxml2.cpp
)

# ... to build the executable.
add_executable( ${EXECUTABLE} src/main.cpp ${CPP_FILES} ${HPP_FILES})

# Test executables
add_executable( test_xml tests/xml.cpp ${CPP_FILES} ${HPP_FILES} )
add_executable( test_state tests/state.cpp ${CPP_FILES} ${HPP_FILES} )
add_executable( test_buildorder tests/buildorder.cpp ${CPP_FILES} ${HPP_FILES} )
add_executable( optimizer tests/optimizer.cpp ${CPP_FILES} ${HPP_FILES} )

add_definitions(-DDEBUG) # Uncomment (to enable) or Comment (to disable) the debug output (see include/Debug.hpp)
