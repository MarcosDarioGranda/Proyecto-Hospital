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

void mostrarMenuPacientes(int socket) {
    int opcion;
    do {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Ver lista de pacientes\n";
        cout << "2. Buscar paciente por ID\n";
        cout << "3. Anyadir nuevo paciente\n";
        cout << "4. Modificar paciente existente\n";
        cout << "5. Eliminar paciente\n";
        cout << "6. Ver citas por paciente\n";
        cout << "7. Ver citas por médico\n";
        cout << "8. Anyadir nueva cita\n";
        cout << "9. Modificar cita existente\n";
        cout << "10. Eliminar cita\n";
        cout << "11. Consultar historial médico de paciente\n";
        cout << "12. Anyadir historial médico\n";
        cout << "13. Modificar historial médico\n";
        cout << "14. Eliminar historial médico\n";

        cout << "15. Salir\n";
        cout << "Seleccione una opcion: ";
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
                cout << "Ingrese ID del paciente para ver sus citas: ";
                getline(cin, input);
                comando = "CONSULTAR_CITAS_PACIENTE " + input;
                break;
            case 7:
                cout << "Ingrese ID del médico para ver sus citas: ";
                getline(cin, input);
                comando = "CONSULTAR_CITAS_MEDICO " + input;
                break;
            case 8:
                cout << "Formato: paciente_id,fecha,medico_id,estado\n";
                cout << "Ingrese los datos de la nueva cita: ";
                getline(cin, input);
                comando = "AÑADIR_CITA " + input;
                break;
            case 9:
                cout << "Formato: id,fecha,estado,medico_id(opcional)\n";
                cout << "Ingrese los datos modificados de la cita: ";
                getline(cin, input);
                comando = "MODIFICAR_CITA " + input;
                break;
            case 10:
                cout << "Ingrese ID de la cita a eliminar: ";
                getline(cin, input);
                comando = "ELIMINAR_CITA " + input;
                break;
            case 11:
                cout << "Ingrese ID del paciente para consultar historial: ";
                getline(cin, input);
                comando = "CONSULTAR_HISTORIAL " + input;
                break;
            case 12:
                cout << "Formato: paciente_id,antecedente\n";
                cout << "Ingrese los datos del historial a añadir: ";
                getline(cin, input);
                comando = "AÑADIR_HISTORIAL " + input;
                break;
            case 13:
                cout << "Formato: id,paciente_id,antecedente\n";
                cout << "Ingrese los datos para modificar historial: ";
                getline(cin, input);
                comando = "MODIFICAR_HISTORIAL " + input;
                break;
            case 14:
                cout << "Ingrese ID del historial a eliminar: ";
                getline(cin, input);
                comando = "ELIMINAR_HISTORIAL " + input;
                break;
            case 15:
                comando = "SALIR";
                cout << "Cerrando conexion...\n";
                break;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        enviarComando(socket, comando);
        cout << "\nRespuesta del servidor:\n";
        cout << recibirRespuesta(socket) << endl;
    } while (opcion != 15);
}
