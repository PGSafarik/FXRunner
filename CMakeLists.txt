CMAKE_MINIMUM_REQUIRED( VERSION 3.7 )
PROJECT( FXRunner )

# The project settings
# EMPTY

# Configure project
# CONFIGURE_FILE(
#   "${PROJECT_SOURCE_DIR}/MakeConfig.h.in"
#   "${PROJECT_BINARY_DIR}/MakeConfig.h"
# )
# EMPTY

# Add directories for project
INCLUDE_DIRECTORIES( "${PROJECT_SOURCE_DIR}/include" )
INCLUDE_DIRECTORIES( "${PROJECT_BINARY_DIR}" )

#INCLUDE_DIRECTORIES( "/opt/foxlib/1.7/64/include" )    # foxtoolkit headers
#LINK_DIRECTORIES( "/opt/foxlib/1.7/64/lib/" )          # foxtoolkit library

# Add to the executable
ADD_EXECUTABLE( fxrunner src/main.cpp src/Application.cpp src/FXIcon_Theme.cpp src/Runner.cpp src/Boxes.cpp 
src/FXBoxFrame.cpp src/History.cpp src/Task.cpp )
TARGET_LINK_LIBRARIES( fxrunner FOX-1.7 CHART-1.7 )

# Install build project
INSTALL( TARGETS fxrunner RUNTIME DESTINATION bin )
