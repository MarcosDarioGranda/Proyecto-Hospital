cmake_minimum_required(VERSION 3.5)
project(Funciones CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(funciones STATIC
    funciones_pacientes.cpp
    funciones_citas.cpp
    funciones_historiales.cpp
)

target_include_directories(funciones PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../lib/include
    ../clases
)