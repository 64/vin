cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(vin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__DEBUG__")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests")
add_subdirectory("third_party/glfw")

file(GLOB SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(vin ${SRCS} "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")

find_package(Freetype REQUIRED)

target_include_directories(vin PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/third_party/glad/include" ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(vin glfw ${FREETYPE_LIBRARIES})
