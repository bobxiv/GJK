cmake_minimum_required (VERSION 2.8)
project (GJK)
# The version number.
set (gjk_VERSION_MAJOR 1)
set (gjk_VERSION_MINOR 0)

#Path a includes y librerias
set(GLUT_INCLUDE "" CACHE PATH "path to GLUT includes")
set(GLUT_LIB "" CACHE PATH "path to GLUT libs")
set(OPENGL_INCLUDE "" CACHE PATH "path to OPENGL includes")
set(OPENGL_LIB "" CACHE PATH "path to OPENGL libs")
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#Agregamos los path de includes
include_directories(${OPENGL_INCLUDE} ${GLUT_INCLUDE})

#Configuracion del proyecto estrictamente. Si luego hay mas de uno...

#Proyecto de ejecutable a generar con codigos
file(GLOB GJK_SOURCES *.cpp *.h)
add_executable(gjk ${GJK_SOURCES})

option(X_86 "Compile and link 32bits. If not 64bits"         ON)


#Librerias a linkear
#Cada una con path completo y nombre de libreria
option(WITH_GLUT "Include GLUT in the project"         ON)

#Librerias a linkear
#Cada una con path completo y nombre de libreria
if( WITH_GLUT )
	set(LIBRARIES ${LIBRARIES} ${GLUT_LIB}glut32.lib )
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

	set(LIBRARIES ${LIBRARIES} ${OPENGL_LIB}${CPUARCHITECTURE}opengl32.lib ${OPENGL_LIB}${CPUARCHITECTURE}glu32.lib )
endif (WITH_OPENGL)

#CPUARCHITECTURE dice a que arquitectura de CPU se compilara
if( X_86 )
	set(CPUARCHITECTURE msvc/)#x86
else( X_86 )
	set(CPUARCHITECTURE msvc_x64/)#x64
endif( X_86 )

target_link_libraries(gjk ${LIBRARIES})