cmake_minimum_required(VERSION 3.2.2)
cmake_policy(SET CMP0026 OLD)
project(CS580)

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
add_subdirectory(ExternalProjects)

# On Visual 2005 and above, this module can set the debug working directory.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ExternalProjects/cmake-modules-master")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if (INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif (INCLUDE_DISTRIB)

include_directories(
	ExternalProjects/glfw-3.2.1/include/GLFW/
	ExternalProjects/glm-0.9.8.5/
	ExternalProjects/glew-1.9.0/include/
	ExternalProjects/soil-1.16/inc
	ExternalProjects/assimp-3.3.1/include
	ExternalProjects/tinyobjloader-1.0.6
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_190
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
set(MY_TARGET "LightingMobile")

file(GLOB_RECURSE MY_SOURCES
    "Sources/*.cpp"
    "Sources/*.hpp"
)

add_executable(${MY_TARGET}
	${MY_SOURCES}
)

target_link_libraries(${MY_TARGET}
	${ALL_LIBS}
)

# ======================================================

# Xcode and Visual Studio working directories
set_target_properties(${MY_TARGET} PROPERTIES
    XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

create_target_launcher(${MY_TARGET} WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)
