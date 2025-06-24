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
        cout << "\n---- Menu Pacientes ----\n";
        cout << "\n1. Ver lista de pacientes\n";
        cout << "2. Buscar paciente por ID\n";
        cout << "3. Anyadir nuevo paciente\n";
        cout << "4. Modificar paciente existente\n";
        cout << "5. Eliminar paciente\n";
        cout << "6. Volver al menu principal\n";
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
                comando = "ANADIR_PACIENTE " + input;
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
                return;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        enviarComando(socket, comando);
        cout << "\nRespuesta del servidor:\n";
        cout << recibirRespuesta(socket) << endl;
    } while (opcion != 6);
}

void mostrarMenuCitas(int socket) {
    int opcion;
    string input, comando;
    do {
        cout << "\n---- Menu de Citas ----\n";
        cout << "\n1. Ver citas por paciente\n";
        cout << "2. Ver citas por medico\n";
        cout << "3. Anyadir nueva cita\n";
        cout << "4. Modificar cita existente\n";
        cout << "5. Eliminar cita\n";
        cout << "6. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Ingrese ID del paciente para ver sus citas: ";
                getline(cin, input);
                comando = "CONSULTAR_CITAS_PACIENTE " + input;
                break;
            case 2:
                cout << "Ingrese ID del médico para ver sus citas: ";
                getline(cin, input);
                comando = "CONSULTAR_CITAS_MEDICO " + input;
                break;
            case 3:
                cout << "Formato: paciente_id,fecha,medico_id,estado\n";
                cout << "Ingrese los datos de la nueva cita: ";
                getline(cin, input);
                comando = "ANADIR_CITA " + input;
                break;
            case 4:
                cout << "Formato: id,fecha,estado,medico_id(opcional)\n";
                cout << "Ingrese los datos modificados de la cita: ";
                getline(cin, input);
                comando = "MODIFICAR_CITA " + input;
                break;
            case 5:
                cout << "Ingrese ID de la cita a eliminar: ";
                getline(cin, input);
                comando = "ELIMINAR_CITA " + input;
                break;
            case 6:
                return;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        enviarComando(socket, comando);
        cout << "\nRespuesta del servidor:\n";
        cout << recibirRespuesta(socket) << endl;

    } while (opcion != 6);
}

void mostrarMenuHistorial(int socket) {
    int opcion;
    string input, comando;
    do {
        cout << "\n---- Menu de Historial Medico ----\n";
        cout << "\n1. Consultar historial de paciente\n";
        cout << "2. Anyadir historial medico\n";
        cout << "3. Modificar historial medico\n";
        cout << "4. Eliminar historial medico\n";
        cout << "5. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Ingrese ID del paciente para consultar historial: ";
                getline(cin, input);
                comando = "CONSULTAR_HISTORIAL " + input;
                break;
            case 2:
                cout << "Formato: paciente_id,antecedente\n";
                cout << "Ingrese los datos del historial a añadir: ";
                getline(cin, input);
                comando = "ANADIR_HISTORIAL " + input;
                break;
            case 3:
                cout << "Formato: id,paciente_id,antecedente\n";
                cout << "Ingrese los datos para modificar historial: ";
                getline(cin, input);
                comando = "MODIFICAR_HISTORIAL " + input;
                break;
            case 4:
                cout << "Ingrese ID del historial a eliminar: ";
                getline(cin, input);
                comando = "ELIMINAR_HISTORIAL " + input;
                break;
            case 5:
                return;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        enviarComando(socket, comando);
        cout << "\nRespuesta del servidor:\n";
        cout << recibirRespuesta(socket) << endl;

    } while (opcion != 5);
}
