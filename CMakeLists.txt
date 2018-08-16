cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
project(vin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__DEBUG__")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} "main.cpp" "glad.c" "configreader.cpp" "options.cpp" "trim.cpp" "messagebox.cpp")
target_link_libraries(vin glfw  X11 ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${X11_LIBRARIES})
