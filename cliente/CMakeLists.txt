cmake_minimum_required(VERSION 3.5)
project(ClienteHospital CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Cliente
    main.cpp
    protocolo.cpp
)

target_include_directories(Cliente PRIVATE
    ../lib/include
)
target_link_libraries(Cliente PRIVATE
    ws2_32
    hospital
)

