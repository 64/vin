cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
project(vin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__DEBUG__")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} "main.cpp" "glad.c" "configreader.cpp" "options.cpp" "trim.cpp" "messagebox.cpp")

if (WIN32)
    target_link_libraries(vin ${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a)
endif (WIN32)

if (UNIX)
    target_link_libraries(vin glfw X11 ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${X11_LIBRARIES})
endif (UNIX)

