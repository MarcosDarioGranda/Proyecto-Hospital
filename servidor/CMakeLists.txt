cmake_minimum_required(VERSION 3.5)
project(ServidorHospital CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Servidor
    main.cpp
    protocolo.cpp
    login.cpp
    funciones_pacientes.cpp
)

target_include_directories(Servidor PRIVATE
    ../lib/include
    ../clases
)

target_link_libraries(Servidor PRIVATE
    ws2_32
    hospital
)

