#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <winsock2.h>
#include "menu.h"

using namespace std;

void enviarComando(int socket, const string& comando) {
    send(socket, comando.c_str(), comando.size(), 0);
}

string recibirRespuesta(int socket) {
    char buffer[2048];
    memset(buffer, 0, sizeof(buffer));
    recv(socket, buffer, sizeof(buffer) - 1, 0);
    return string(buffer);
}

void mostrarMenu(int socket) {
    int opcion;
    do {
        cout << "\n--- Menú Principal ---\n";
        cout << "1. Ver lista de pacientes
2. Buscar paciente por ID
3. Añadir nuevo paciente
4. Modificar paciente existente
5. Eliminar paciente
6. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        string input, comando;
        switch (opcion) {
            case 1:
                comando = "LISTAR_PACIENTES";
                break;
            case 2:
                cout << "Ingrese ID del paciente: ";
                getline(cin, input);
                comando = "BUSCAR_PACIENTE " + input;
                break;
            case 3:
                cout << "Formato: nombre,fecha_nac,direccion,tlfn\n";
                cout << "Ingrese los datos del nuevo paciente: ";
                getline(cin, input);
                comando = "AÑADIR_PACIENTE " + input;
                break;
            case 4:
                cout << "Formato: id,nombre,fecha_nac,direccion,tlfn\n";
                cout << "Ingrese los datos modificados del paciente: ";
                getline(cin, input);
                comando = "MODIFICAR_PACIENTE " + input;
                break;
            case 5:
                cout << "Ingrese ID del paciente a eliminar: ";
                getline(cin, input);
                comando = "ELIMINAR_PACIENTE " + input;
                break;
            case 6:
                comando = "SALIR";
                cout << "Cerrando conexión...\n";
                break;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        enviarComando(socket, comando);
        cout << "\nRespuesta del servidor:\n";
        cout << recibirRespuesta(socket) << endl;
    } while (opcion != 6);
}
