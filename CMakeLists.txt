CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )
PROJECT( FXRunner VERSION 0.1 DESCRIPTION "GUI application launcher for Linux with history" )

# The project settings
SET( BIN_NAME "fxrunner" )                   # Executable binnary filename
SET( CMAKE_INSTALL_PREFIX "/usr" )  # Install target directory
SET( LIB_FOX   "/opt/foxlib/1.7/84" )       # Fox toolkit path
SET( LIB_FXGHI "/opt/foxghi/0.5/2"  )       # Fox GHI path
SET( CMAKE_CXX_STANDARD 11 )                # C++ Standard
SET( CMAKE_CXX_STANDARD_REQUIRED ON )       # C++ Standard
#add_compile_options(-Wall -Wextra -pedantic -Werror)
#add_compile_options(-Wall -pedantic)

# Configure project
#CONFIGURE_FILE( MakeConfig.h.in MakeConfig.h )
FILE( GLOB SOURCES src/*.cpp src/core/*.cpp src/widgets/*.cpp )                          # List of implementation files
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )

# Add directories for project dependecies
INCLUDE_DIRECTORIES( "${LIB_FOX}/include/" )
LINK_DIRECTORIES(    "${LIB_FOX}/lib/" )
INCLUDE_DIRECTORIES( "${LIB_FXGHI}/include/" )
LINK_DIRECTORIES(    "${LIB_FXGHI}/lib/" )

# Add to the executable
ADD_EXECUTABLE( ${BIN_NAME} ${SOURCES} )
TARGET_LINK_LIBRARIES( ${BIN_NAME} loki FOX-1.7 FXGHI )

# Install build project
INSTALL( TARGETS ${BIN_NAME} RUNTIME DESTINATION bin )
INSTALL( DIRECTORY data DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME} )		# Globalni data
