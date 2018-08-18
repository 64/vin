cmake_minimum_required(VERSION 3.6)

#set(CMAKE_CXX_STANDARD 17)
project(vin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__DEBUG__")


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build GLFW tests")
add_subdirectory("third_party/glfw")

add_definitions(-DGLFW_INCLUDE_NONE)

option(WIN64 "Use Win64 FreeType Binaries (Windows Only)" OFF)
set(FT_WIN_PREFIX "win32")
if (WIN64)
	set(FT_WIN_PREFIX "win64")
endif (WIN64)

file(GLOB SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(vin ${SRCS} "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")

if(WIN32)
    set(ENV{FREETYPE_DIR} "${CMAKE_SOURCE_DIR}/third_party/freetype_win")

    # The github repo doesn't have the binaries in quite the right place for us to use.
    # freetype.lib needs to be copied from win64/ so find_package can locate it.
    # freetype.dll needs to be copied to where the executable will be so it can be loaded at run time.
    # if(NOT (EXISTS "${CMAKE_BINARY_DIR}/freetype.dll" AND EXISTS "${CMAKE_SOURCE_DIR}/third_party/freetype_win/freetype.lib"))
        file(COPY "${CMAKE_SOURCE_DIR}/third_party/freetype_win/${FT_WIN_PREFIX}/freetype.dll" DESTINATION "${CMAKE_BINARY_DIR}")
        file(COPY "${CMAKE_SOURCE_DIR}/third_party/freetype_win/${FT_WIN_PREFIX}/freetype.lib" DESTINATION "${CMAKE_SOURCE_DIR}/third_party/freetype_win")
        message(STATUS "Successfully copied FreeType libraries")
    # endif()
endif()

find_package(Freetype REQUIRED)

target_include_directories(vin PUBLIC "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/third_party/glad/include" ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(vin glfw ${FREETYPE_LIBRARIES})
