cmake_minimum_required(VERSION 3.10)
project(RotationExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include GLFW and GLM manually
include_directories(
    /usr/include  # Add GLM headers
    ../include
    ../../include    # Other include directories
)

# Set source files
set(SOURCES
    ../src/shader.cpp
    rotation_ex.cpp
    ../../src/glad.c
)

# Create executable
add_executable(rotation_ex ${SOURCES})

# Link libraries
target_link_libraries(rotation_ex
    ${OPENGL_LIBRARIES} 
    glfw 
)
