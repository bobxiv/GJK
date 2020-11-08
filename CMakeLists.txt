cmake_minimum_required (VERSION 2.8)
project (GJK)
# Version
set (gjk_VERSION_MAJOR 1)
set (gjk_VERSION_MINOR 0)

IF (WIN32)
    option(SFML_STATIC_LIBRARIES "Usar biblioteca estática para SFML" ON)
ELSE()# Linux find_package doesn't find static libs
    option(SFML_STATIC_LIBRARIES "Usar biblioteca estática para SFML" OFF)
ENDIF()

# Librerias a linkear e includes
set(LIBRARIES)

IF (WIN32)
    find_package(SFML COMPONENTS system window graphics main REQUIRED)
    set(LIBRARIES ${LIBRARIES} sfml-window sfml-system sfml-main sfml-graphics)
ELSE()# Linux find_package doesn't find main component
    find_package(SFML COMPONENTS system window graphics REQUIRED)
    set(LIBRARIES ${LIBRARIES} sfml-window sfml-system sfml-graphics)
ENDIF()

# Proyecto de ejecutable a generar con codigos
file(GLOB GJK_SOURCES *.cpp *.h)
add_executable(gjk ${GJK_SOURCES})
target_link_libraries(gjk ${LIBRARIES})

# Capetas de salida
IF (WIN32)
    set_target_properties(gjk PROPERTIES PDB_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
    set_target_properties(gjk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/$<CONFIG>")
ELSE()# In Linux just put them in Linux folder
    set_target_properties(gjk PROPERTIES PDB_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/Linux")
    set_target_properties(gjk PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/Linux")
ENDIF()