cmake_minimum_required(VERSION 3.10)

# Required version of cpp
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project("OpenGL_Lab" VERSION 0.1 LANGUAGES CXX C)

message(STATUS "The build type is set as ${CMAKE_BUILD_TYPE}.")
# set build type as debug
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# create a list to keep source files in a place
set(SRCS "src/main.cpp")

# append lists to add source files
list(APPEND SRCS "src/lab1.cpp")
list(APPEND SRCS "src/lab2.cpp")
list(APPEND SRCS "src/lab3.cpp")
list(APPEND SRCS "src/lab4.cpp")
list(APPEND SRCS "src/lab5.cpp")
list(APPEND SRCS "src/lab6.cpp")



if (APPLE)
    # create bundle on apple compiles
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS} )
else()
    add_executable(${PROJECT_NAME} ${SRCS})
endif()

set(OpenGL_GL_PREFERENCE "LEGACY")

find_package(OpenGL)

include_directories(${OPENGL_INCLUDE_DIRS})

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" PUBLIC include)

add_subdirectory("./lib")

target_link_libraries("${PROJECT_NAME}" ${OPENGL_LIBRARIES} glfw glad glm::glm)
