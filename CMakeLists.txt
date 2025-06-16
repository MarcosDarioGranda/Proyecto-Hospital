cmake_minimum_required(VERSION 3.15)
project(ProyectoHospital)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opcional: establecer ruta común de includes
include_directories(lib/include clases)

# Añadir subdirectorios
add_subdirectory(lib)
add_subdirectory(cliente)
add_subdirectory(servidor)