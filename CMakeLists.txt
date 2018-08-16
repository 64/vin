cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
project(vin)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} "main.cpp" "glad.c")
target_link_libraries(vin glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})
