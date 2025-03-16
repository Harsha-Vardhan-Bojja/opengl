cmake_minimum_required(VERSION 3.10)
project(Car_Game)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set source files (relative to texture/ directory)
set(SOURCES
    ./src/shader.cpp
    ./src/main.cpp
    ../../src/glad.c  # Adjust path since CMakeLists is inside texture/
)

# Include directories (relative to texture/)
include_directories(
    ./include   # Adjust path for include directory
    /usr/include
    ../../include/
)

# Create executable
add_executable(Car_game ${SOURCES})

# Link libraries
target_link_libraries(Car_game 
    ${OPENGL_LIBRARIES} 
    glfw 
)

