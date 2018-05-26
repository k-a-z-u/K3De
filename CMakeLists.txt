CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(K3De)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

IF (NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "No build type selected. Default to RELEASE")
	SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(X11 REQUIRED)

OPTION(WITH_PNG "Build with PNG image support" OFF)
IF(WITH_PNG)
	MESSAGE(STATUS "PNG image support: ENABLED")
	add_definitions(-DWITH_PNG)
	SET(EXTRA_LIBS ${EXTRA_LIBS} png)
ELSE()
	MESSAGE(STATUS "PNG image support: DISABLED")
ENDIF()


OPTION(WITH_JPEG "Build with JPEG image support" OFF)
IF(WITH_JPEG)
	MESSAGE(STATUS "JPEG image support: ENABLED")
	add_definitions(-DWITH_JPEG)
	SET(EXTRA_LIBS ${EXTRA_LIBS} jpeg)
ELSE()
	MESSAGE(STATUS "JPEG image support: DISABLED")
ENDIF()

OPTION(WITH_TEST "Build with Test-Cases" OFF)
IF(WITH_TEST)
	MESSAGE(STATUS "Test-Caseses: ENABLED")
	add_definitions(-DWITH_TESTS)
	SET(EXTRA_LIBS ${EXTRA_LIBS} gtest)
ELSE()
	MESSAGE(STATUS "Test-Cases: DISABLED")
ENDIF()




INCLUDE_DIRECTORIES(
	/apps/workspaces/
	../
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
	-DWITH_DEBUG

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
	pthread
	${EXTRA_LIBS}
   #${SDL_LIBRARY}
   #${SDLIMAGE_LIBRARY}
   #${SDLGFX_LIBRARY}
   #${SDLTTF_LIBRARY}
   #${SDLMIXER_LIBRARY}
   #${OPENGL_LIBRARIES}
   #${X11_LIBRARIES}
)
