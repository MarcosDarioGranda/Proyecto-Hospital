#include <iostream>
#include <string>
#include <winsock2.h>
#include "menu.h"

#include "protocolo.h"          
using namespace std;

void loginYMenu(int socket) {
    string usuario, clave;
    cout << "Usuario: ";
    getline(cin, usuario);
    cout << "Contrasenya: ";
    getline(cin, clave);

    string comando = formatRequest(CMD_LOGIN, {usuario, clave});
    send(socket, comando.c_str(), comando.size(), 0);

    char buffer[2048];
    int n = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) {
        cerr << "Error en la conexion o respuesta.\n";
        return;
    }
    buffer[n] = '\0';
    string respuesta(buffer);

    if (respuesta.rfind("OK|", 0) == 0) {
        string rol = respuesta.substr(3);
        cout << "Login correcto. Rol: " << rol << endl;
        mostrarMenuPacientes(socket); // Entramos al menú
    } else {
        cout << "Login fallido: " << respuesta << endl;
    }
}
