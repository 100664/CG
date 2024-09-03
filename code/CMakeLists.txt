cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(phase4)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define os executáveis a serem construídos neste projecto
add_executable(engine.exe ./engine/engine.cpp)
add_executable(generator.exe ./generator/generator.cpp  ./generator/plane.cpp ./generator/box.cpp ./generator/cone.cpp ./generator/sphere.cpp ./generator/ring.cpp ./generator/bezier.cpp)

# Biblioteca das figuras e dos pontoss
add_library(
    utils_lib
    ./utils/point.cpp
    ./utils/point.hpp
    ./utils/figure.cpp
    ./utils/figure.hpp
	./utils/triangle.cpp
	./utils/triangle.hpp
	./utils/matrix.cpp
	./utils/matrix.hpp
)

# Biblioteca de parsing de xml
add_library(
    tinyxml_lib
    ./tinyxml/tinystr.cpp
    ./tinyxml/tinyxmlerror.cpp
    ./tinyxml/tinyxmlparser.cpp
    ./tinyxml/tinystr.h
    ./tinyxml/tinyxml.h
    ./tinyxml/tinyxml.cpp
)

# Biblioteca de configuração do engine.
add_library(
    config_lib
    ./engine/xml_parser.cpp
    ./engine/xml_parser.hpp
	./engine/catmull.cpp
	./engine/catmull.hpp
    ./engine/color.cpp
    ./engine/color.hpp
    ./engine/lights.cpp
    ./engine/lights.hpp
)

# Link tinyxml_lib com config_lib
target_link_libraries(config_lib tinyxml_lib)

# Associa a biblioteca das figuras e dos pontos aos executáveis
target_link_libraries(generator.exe utils_lib)
target_link_libraries(engine.exe utils_lib tinyxml_lib config_lib)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	
	include_directories(${TOOLKITS_FOLDER}/glut )
	#target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
	#									  ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(generator.exe ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(engine.exe ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	
	
	#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
	
	target_link_libraries(generator.exe ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	target_link_libraries(engine.exe ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)