cmake_minimum_required(VERSION 3.23)
project(gravitySimulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)

include_directories(include/glm)
include_directories(include/glm/detail)
include_directories(include/glm/ext)
include_directories(include/glm/gtc)
include_directories(include/glm/gtx)
include_directories(include/glm/simd)

include_directories(include/glad)
include_directories(include/GLFW)
include_directories(include/KHR)

add_executable(gravitySimulator main.cpp)


target_link_libraries(gravitySimulator ${PROJECT_SOURCE_DIR}/lib/libglfw.3.3.dylib)
target_link_libraries(gravitySimulator ${PROJECT_SOURCE_DIR}/glad.c)
