Y luego en consola (desde servidor/):

bash
Copiar
Editar
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .

Si prefieres compilación manual con MinGW:

bash
Copiar
Editar
g++ main.cpp protocolo.cpp -I../lib -L../lib -lhospital -lws2_32 -o servidor.exe