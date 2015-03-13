cmake_minimum_required( VERSION 2.8 FATAL_ERROR )

set( EXECUTABLE sc2sim )
set( HEADER_FOLDER include )
set( CPP_FOLDER src )

include_directories( ${HEADER_FOLDER} )
include_directories( ${CPP_FOLDER} )

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
endif()

# Output some properties for easy debugging
message( "C COMPILER   -> " ${CMAKE_C_COMPILER} )
message( "CXX COMPILER -> " ${CMAKE_CXX_COMPILER} )
message( "C FLAGS:     -> " ${CMAKE_C_FLAGS} )
message( "CXX FLAGS:   -> " ${CMAKE_CXX_FLAGS} )

# Add all the header files ...
set( HPP_FILES
    include/tinyxml2.h
    include/ConfigParser.h
    include/State.h
)

# ... and source files ...
set( CPP_FILES
  src/tinyxml2.cpp
  src/ConfigParser.cpp
  src/State.cpp
)

# ... to build the executable.
add_executable( ${EXECUTABLE} src/main.cpp ${CPP_FILES} ${HPP_FILES} )

add_executable( test_xml tests/xml.cpp ${CPP_FILES} ${HPP_FILES} )
add_executable( test_state tests/state.cpp ${CPP_FILES} ${HPP_FILES} )

# Uncomment (to enable) or Comment (to disable) the debug output (see includes/Debug.hpp)
add_definitions(-DDEBUG)
