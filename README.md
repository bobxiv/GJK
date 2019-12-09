# GJK

Una implementación simple del algoritmo GJK junto con una aplicación pequeña para mostrar el proceso de cálculo a través de la suma de Minkowski.

## Tabla de Contenidos

- [GJK](#gjk)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Dependencias](#dependencias)
  - [Building](#building)

## Dependencias

El código sólo utiliza SFML v2.5.1.

## Building

Para compilar el proyecto, primero se debe crear la solución con [CMake](https://cmake.org/). Actualmente se usa **CMake v3.16**. Seguir los siguientes pasos:

1. **SFML**: Inicializar el sub-modulo de SFML y compilar el proyecto utilizando CMake. Usamos esta configuración:
   * BUILD_SHARED_LIBS -> Off.
   * SFML_INSTALL_PKGCONFIG_FILES -> On. Es necesario para encontrar el paquete de SFML.
2. **Crear proyecto y compilar**: Crear el proyecto utilizando CMake y compilar. Se puede usar este comando cuando se está parado en el root del repo:

```bash
cmake -S . -B build/
```

Si se quiere utilizar una interfaz gráfica se puede usar **cmake-gui** en Windows. Por otro lado una interfaz del shell más amigable es con el comando:

```bash
ccmake -S . -B build/
```

Actualmente, estamos usando **Visual Studio 2019**.
