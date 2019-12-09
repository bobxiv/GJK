cmake_minimum_required (VERSION 2.8)
project (GJK)
# The version number.
set (gjk_VERSION_MAJOR 1)
set (gjk_VERSION_MINOR 0)

#Path a includes y librerias
set(GLUT_INCLUDE "" CACHE PATH "path to GLUT includes")
set(GLUT_LIB "" CACHE PATH "path to GLUT libs")
set(GLUT_LIB_NAME "glut32" CACHE PATH "name of the glut library")
set(OPENGL_INCLUDE "" CACHE PATH "path to OPENGL includes")
set(OPENGL_LIB "" CACHE PATH "path to OPENGL libs")

set(LIBRARIES_DIR)
set(LIBRARIES)
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#Agregamos los path de includes
include_directories(${OPENGL_INCLUDE} ${GLUT_INCLUDE})

#Configuracion del proyecto estrictamente. Si luego hay mas de uno...
option(X_86 "Compile and link 32bits. If not 64bits"         ON)


#Librerias a linkear
#Cada una con path completo y nombre de libreria
option(WITH_GLUT "Include GLUT in the project"         ON)

#Librerias a linkear
#Cada una con path completo y nombre de libreria
if( WITH_GLUT )
	set(LIBRARIES_DIR ${LIBRARIES_DIR} ${GLUT_LIB})
	set(LIBRARIES ${LIBRARIES} ${GLUT_LIB_NAME} )
endif (WITH_GLUT)

option(WITH_OPENGL "Include OpenGL in the project"         ON)

#Librerias a linkear
#Cada una con path completo y nombre de libreria
if( WITH_OPENGL )

	if( X_86 )
		set(CPUARCHITECTURE )#x86
	else( X_86 )
		set(CPUARCHITECTURE x64/)#x64
	endif( X_86 )

	set(LIBRARIES_DIR ${LIBRARIES_DIR} ${OPENGL_LIB})
	set(LIBRARIES ${LIBRARIES} ${CPUARCHITECTURE}opengl32 ${CPUARCHITECTURE}glu32 )
endif (WITH_OPENGL)

#CPUARCHITECTURE dice a que arquitectura de CPU se compilara
if( X_86 )
	set(CPUARCHITECTURE msvc/)#x86
else( X_86 )
	set(CPUARCHITECTURE msvc_x64/)#x64
endif( X_86 )

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)

set(LIBRARIES_DIR ${LIBRARIES_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib)
set(LIBRARIES ${LIBRARIES} libsfml-window-s-d libsfml-system-s-d libsfml-main-d.lib libsfml-graphics-s-d)

include_directories(${INCLUDE_DIR})
link_directories(${LIBRARIES_DIR})

#Proyecto de ejecutable a generar con codigos
file(GLOB GJK_SOURCES *.cpp *.h)
add_executable(gjk ${GJK_SOURCES})
target_link_libraries(gjk ${LIBRARIES})