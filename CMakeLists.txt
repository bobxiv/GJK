cmake_minimum_required (VERSION 2.8)
project (GJK)
# Version
set (gjk_VERSION_MAJOR 1)
set (gjk_VERSION_MINOR 0)

option(SFML_STATIC_LIBRARIES "Usar biblioteca estática para SFML" ON)

# Librerias a linkear e includes
set(LIBRARIES)

find_package(SFML COMPONENTS system window graphics main REQUIRED)
set(LIBRARIES ${LIBRARIES} sfml-window sfml-system sfml-main sfml-graphics)

# Proyecto de ejecutable a generar con codigos
file(GLOB GJK_SOURCES *.cpp *.h)
add_executable(gjk ${GJK_SOURCES})
target_link_libraries(gjk ${LIBRARIES})