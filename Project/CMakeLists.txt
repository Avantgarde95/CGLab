cmake_minimum_required(VERSION 3.2.2)
cmake_policy(SET CMP0026 OLD)
project(Gallery)

find_package(OpenGL REQUIRED)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory")
endif ()

if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()

if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()

# Compile external dependencies.
add_subdirectory(Libraries)

# On Visual 2005 and above, this module can set the debug working directory.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Libraries/cmake-modules-master")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if (INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif (INCLUDE_DISTRIB)

include_directories(
        Libraries/glfw-3.2.1/include/GLFW
        Libraries/glm-0.9.8.5
        Libraries/glew-1.9.0/include
        Libraries/tinyobjloader-1.0.6
        Libraries/soil-1.16/inc
        .
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

# ======================================================

set(CMAKE_CXX_STANDARD 11)

set(HW0_TARGET "HW0")
set(HW1_TARGET "HW1")
set(HW2_TARGET "HW2")
set(HW3_TARGET "HW3")

file(
        GLOB_RECURSE HW0_SOURCES
        "HW0/*.cpp"
        "HW0/*.hpp"
)

file(
        GLOB_RECURSE HW1_SOURCES
        "HW1/*.cpp"
        "HW1/*.hpp"
)

file(
        GLOB_RECURSE HW2_SOURCES
        "HW2/*.cpp"
        "HW2/*.hpp"
)

file(
        GLOB_RECURSE HW3_SOURCES
        "HW3/*.cpp"
        "HW3/*.hpp"
)

add_executable(
        ${HW0_TARGET}
        ${HW0_SOURCES}
)

add_executable(
        ${HW1_TARGET}
        ${HW1_SOURCES}
)

add_executable(
        ${HW2_TARGET}
        ${HW2_SOURCES}
)

add_executable(
        ${HW3_TARGET}
        ${HW3_SOURCES}
)

target_link_libraries(
        ${HW0_TARGET}
        ${OPENGL_LIBRARY}
        glfw
        GLEW_190
)

target_link_libraries(
        ${HW1_TARGET}
        ${OPENGL_LIBRARY}
        glfw
        GLEW_190
)

target_link_libraries(
        ${HW2_TARGET}
        ${OPENGL_LIBRARY}
        glfw
        GLEW_190
)

target_link_libraries(
        ${HW3_TARGET}
        ${OPENGL_LIBRARY}
        glfw
        GLEW_190
        soil
)

# ======================================================

# Xcode and Visual Studio working directories
set_target_properties(
        ${HW0_TARGET} PROPERTIES
        XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/HW0/Skeleton/"
)

create_target_launcher(
        ${HW0_TARGET} WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/HW0/Skeleton/"
)

set_target_properties(
        ${HW1_TARGET} PROPERTIES
        XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/HW1/Skeleton/"
)

create_target_launcher(
        ${HW1_TARGET} WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/HW1/Skeleton/"
)

set_target_properties(
        ${HW2_TARGET} PROPERTIES
        XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/HW2/"
)

create_target_launcher(
        ${HW2_TARGET} WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/HW2/"
)

set_target_properties(
        ${HW3_TARGET} PROPERTIES
        XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/HW3/"
)

create_target_launcher(
        ${HW3_TARGET} WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/HW3/"
)
