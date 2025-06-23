#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "protocolo.h"
#include "menu_login.h"
#include "menu.h"

void mostrarMenuPacientes(int socket);
void mostrarMenuCitas(int socket);
void mostrarMenuHistorial(int socket);


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 2048

using namespace std;

static unordered_map<int, vector<string>> cacheHistorial;

int main() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cerr << "WSAStartup fallo: " << iResult << "\n";
        return 1;
    }

    struct addrinfo hints{}, *serverInfo = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo(SERVER_ADDRESS, DEFAULT_PORT, &hints, &serverInfo);
    if (iResult != 0) {
        cerr << "getaddrinfo fallo: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    SOCKET connSock = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (connSock == INVALID_SOCKET) {
        cerr << "socket() fallo: " << WSAGetLastError() << "\n";
        freeaddrinfo(serverInfo);
        WSACleanup();
        return 1;
    }

    iResult = connect(connSock, serverInfo->ai_addr, (int)serverInfo->ai_addrlen);
    freeaddrinfo(serverInfo);
    if (iResult == SOCKET_ERROR) {
        cerr << "connect() fallo: " << WSAGetLastError() << "\n";
        closesocket(connSock);
        WSACleanup();
        return 1;
    }

    cout << "Conectado al servidor " << SERVER_ADDRESS << ":" << DEFAULT_PORT << "\n";

    // Login y luego mostrar menú completo
    string usuario, clave, rol;
    cout << "Usuario: ";
    getline(cin, usuario);
    cout << "Contrasenya: ";
    getline(cin, clave);

    string loginCmd = "LOGIN|" + usuario + "|" + clave + "\r\n";
    send(connSock, loginCmd.c_str(), loginCmd.length(), 0);

    char recvbuf[BUFFER_SIZE];
    iResult = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
    if (iResult <= 0) {
        cerr << "Error al recibir respuesta de login.\n";
        closesocket(connSock);
        WSACleanup();
        return 1;
    }
    recvbuf[iResult] = '\0';
    string respuesta(recvbuf);

    if (respuesta.rfind("OK|", 0) != 0) {
        cout << "Login fallido: " << respuesta << "\n";
        closesocket(connSock);
        WSACleanup();
        return 1;
    }

    cout << "Login correcto. Rol: " << respuesta.substr(3) << "\n";

    bool salir = false;
    while (!salir) {
        cout << "\n1) Consultar historial"
         << "\n2) Agregar historial"
         << "\n3) Gestion de pacientes"
         << "\n4) Gestion de citas"
         << "\n5) Gestion de historial"
         << "\n6) Salir\n> ";
        int opcion; cin >> opcion;
        cin.ignore();

        string request;
        switch (opcion) {
            case 1: {
                int id;
                cout << "ID Paciente: ";
                cin >> id;

                auto it = cacheHistorial.find(id);
                if (it != cacheHistorial.end()) {
                    cout << "(desde cache)\n";
                    for (auto &linea : it->second)
                        cout << linea << "\n";
                } else {
                    request = formatRequest(CMD_CONSULTA_HISTORIAL, { to_string(id) });
                    send(connSock, request.c_str(), (int)request.size(), 0);

                    int n = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
                    recvbuf[n] = '\0';
                    string respuesta(recvbuf);

                    vector<string> lineas;
                    if (respuesta.rfind("OK|", 0) == 0) {
                        string datos = respuesta.substr(3);
                        istringstream ss(datos);
                        string linea;
                        while (getline(ss, linea))
                            if (!linea.empty()) lineas.push_back(linea);
                        cacheHistorial[id] = lineas;
                        for (auto &l : lineas)
                            cout << l << "\n";
                    } else {
                        cout << "Respuesta: " << respuesta << "\n";
                    }
                }
                break;
            }
            case 2: {
                int id;
                cout << "ID Paciente: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Antecedente: ";
                string antecedente;
                getline(cin, antecedente);

                request = formatRequest(CMD_AGREGAR_HISTORIAL, { to_string(id), antecedente });
                send(connSock, request.c_str(), (int)request.size(), 0);
                int n = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
                recvbuf[n] = '\0';
                cout << "Respuesta: " << recvbuf;
                break;
            }
            case 3:
                mostrarMenuPacientes(connSock);
                break;
            case 4:
                mostrarMenuCitas(connSock);
                break;
            case 5:
                mostrarMenuHistorial(connSock);
                break;
            case 6:
                request = formatRequest(CMD_SALIR, {});
                send(connSock, request.c_str(), (int)request.size(), 0);
                salir = true;
                break;
            default:
                cout << "Opcion no valida.\n";
        }
    }

    closesocket(connSock);
    WSACleanup();
    return 0;
}
