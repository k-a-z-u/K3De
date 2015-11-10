CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(openGL)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "No build type selected. Default to RELEASE")
    SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

# TODO: Check if USE_OPENNI has been set and set it for the compilation.

# TODO: Check compiler version to be at least g++4.7 or clang++3.1.


FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(FMODEX REQUIRED)
FIND_PACKAGE(X11 REQUIRED)


INCLUDE_DIRECTORIES(
	/apps/workspaces/

)

FILE(GLOB HEADERS
				./*.glsl
				./*/*.glsl
				./*/*/*.glsl
				./*.h
				./*/*.h
				./*/*/*.h
				./*/*/*/*.h
				)

FILE(GLOB SOURCES
				./*.cpp
				./*/*.cpp
				./*/*/*.cpp
				./*/*/*/*.cpp
				./*.c
				./*/*.c
				./*/*/*.c
				./*/*/*/*.c
				)

ADD_DEFINITIONS(
	-Wall
	-Wextra
	-Werror=return-type
	-std=c++11
	#-DGLEW_STATIC

	-g3
	-O0
	-DWITH_ASSERTIONS
	-DWITH_TESTS

	#-O2
)

# build a binary file
ADD_EXECUTABLE(
	${PROJECT_NAME}
	${HEADERS}
	${SOURCES}
)

#SET(CMAKE_C_FLAGS_DEBUG		"-DHAVE_FCNTL_H -DGLEW_STATIC -DFREEGLUT_STATIC -O0 -g -DDEBUG")
#SET(CMAKE_C_FLAGS_RELEASE	"-DHAVE_FCNTL_H -DGLEW_STATIC -DFREEGLUT_STATIC -O2 -g0 -march=native")

#SET(CMAKE_CXX_FLAGS_DEBUG	"-std=c++0x -O0 -g -DDEBUG")
#SET(CMAKE_CXX_FLAGS_RELEASE	"-std=c++0x -O2 -g0 -march=native")

# create a static library for our engine
#ADD_LIBRARY(openglEngine STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

TARGET_LINK_LIBRARIES(
	${PROJECT_NAME}
	GLEW
	GL
	glfw
	png
	jpeg
	gtest
   #${SDL_LIBRARY}
   #${SDLIMAGE_LIBRARY}
   #${SDLGFX_LIBRARY}
   #${SDLTTF_LIBRARY}
   #${SDLMIXER_LIBRARY}
   #${OPENGL_LIBRARIES}
   #${X11_LIBRARIES}
)
