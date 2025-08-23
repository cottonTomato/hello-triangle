cmake_minimum_required(VERSION 3.28)

# -- Include Guard

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

# -- Project

project(
    GLFWTemplate
    VERSION 0.1.0
    DESCRIPTION "Template for OpenGL app"
    LANGUAGES C CXX
)

# -- Options

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -fexceptions
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

# -- Libs

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm 1.0.1 REQUIRED)

# -- Executable

add_executable(
    ${PROJECT_NAME}_exe
    src/main.cpp
    src/Shader.cpp
    src/stb_image.cpp
    src/glad.c
)
target_include_directories(${PROJECT_NAME}_exe PRIVATE include)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE glfw OpenGL::GL glm::glm)

set_target_properties(${PROJECT_NAME}_exe PROPERTIES OUTPUT_NAME main)
target_compile_features(${PROJECT_NAME}_exe PRIVATE cxx_std_20 c_std_99)
