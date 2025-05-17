cmake_minimum_required(VERSION 3.5)
project(hospital_lib C)

# Aquí sólo compilamos la librería
add_library(hospital STATIC
    src/bd/sqlite3.c
    src/entidades/paciente.c
    src/entidades/historial.c
    src/entidades/cita.c
    src/entidades/usuario.c
    src/menu/funcionesMenu.c
    src/menu/log.c
    src/menu/menus.c
    src/hospital.c
)

target_include_directories(hospital PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)