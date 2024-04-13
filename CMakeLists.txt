cmake_minimum_required(VERSION 3.18)
project(OpenGlTest)

# Add your source files here
set(SOURCES
    src/main.cpp
)

# Add any additional include directories here
#include_directories(include)

# Add any additional library directories here
#link_directories(lib)

# Add any additional libraries here
set(LIBRARIES
#    mylib
)

# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add your executable target here
add_executable(ogtest ${SOURCES})

# Link any libraries to the executable
target_link_libraries(ogtest ${LIBRARIES})
