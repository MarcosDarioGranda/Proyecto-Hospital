// cliente/main.cpp
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501   // Windows XP o superior
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include "protocolo.h"
#include <limits>
#include <sstream>


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 2048

using namespace std;

#include <unordered_map>
#include <vector>
#include <string>

// Caché: mapea ID de paciente → lista de líneas de historial
static unordered_map<int, vector<string>> cacheHistorial;



int main() {
    WSADATA wsaData;
    int iResult;

    // 1) Inicializar Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cerr << "WSAStartup falló: " << iResult << "\n";
        return 1;
    }

    // 2) Resolver dirección del servidor
    struct addrinfo hints{}, *serverInfo = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo(SERVER_ADDRESS, DEFAULT_PORT, &hints, &serverInfo);
    if (iResult != 0) {
        cerr << "getaddrinfo falló: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    // 3) Crear socket
    SOCKET connSock = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (connSock == INVALID_SOCKET) {
        cerr << "socket() falló: " << WSAGetLastError() << "\n";
        freeaddrinfo(serverInfo);
        WSACleanup();
        return 1;
    }

    // 4) Conectar al servidor
    iResult = connect(connSock, serverInfo->ai_addr, (int)serverInfo->ai_addrlen);
    freeaddrinfo(serverInfo);
    if (iResult == SOCKET_ERROR) {
        cerr << "connect() falló: " << WSAGetLastError() << "\n";
        closesocket(connSock);
        WSACleanup();
        return 1;
    }
    cout << "Conectado al servidor " << SERVER_ADDRESS << ":" << DEFAULT_PORT << "\n";

    // 5) Menú y bucle de interacción
    bool salir = false;
    char recvbuf[BUFFER_SIZE];
    while (!salir) {
        cout << "\n1) Consultar historial\n2) Agregar historial\n3) Salir\n> ";
        int opcion; cin >> opcion;

        string request;
        switch (opcion) {
            case 1: {
                int id;
                cout << "ID Paciente: ";
                cin >> id;

                // 2.1 Si ya está en caché, lo mostramos y no pedimos al servidor
                auto it = cacheHistorial.find(id);
                if (it != cacheHistorial.end()) {
                    cout << "(desde caché)\n";
                    for (auto &linea : it->second) {
                        cout << linea << "\n";
                    }
                } else {
                    // 2.2 No está en caché: enviamos petición
                    request = formatRequest(CMD_CONSULTA_HISTORIAL, { to_string(id) });
                    send(connSock, request.c_str(), (int)request.size(), 0);

                    // 2.3 Recibimos la respuesta completa
                    int n = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
                    recvbuf[n] = '\0';
                    string respuesta(recvbuf);

                    // 2.4 Parseamos líneas de datos (descartando el prefijo "OK|")
                    vector<string> lineas;
                    if (respuesta.rfind("OK|", 0) == 0) {
                        // cortamos el "OK|"
                        string datos = respuesta.substr(3);
                        istringstream ss(datos);
                        string linea;
                        while (getline(ss, linea)) {
                            if (!linea.empty()) lineas.push_back(linea);
                        }
                    } else {
                        // en caso de ERR|...
                        cout << "Respuesta: " << respuesta << "\n";
                        break;
                    }

                    // 2.5 Guardamos en caché y mostramos
                    cacheHistorial[id] = lineas;
                    for (auto &l : lineas) {
                        cout << l << "\n";
                    }
                }
                break;
            }

            case 2: {
                int id; 
                cout << "ID Paciente: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el '\n'  
                cout << "Antecedente: ";
                string antecedente;
                getline(cin, antecedente);

                request = formatRequest(CMD_AGREGAR_HISTORIAL, {
                to_string(id),
                antecedente
                });
                break;
            }
            case 3:
                request = formatRequest(CMD_SALIR, {});
                salir = true;
                break;
            default:
                cout << "Opción no válida.\n";
                continue;
        }

        // 6) Enviar y recibir
        iResult = send(connSock, request.c_str(), (int)request.size(), 0);
        if (iResult == SOCKET_ERROR) {
            cerr << "send() falló: " << WSAGetLastError() << "\n";
            break;
        }

        iResult = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            cout << "Respuesta: " << recvbuf;
        } else if (iResult == 0) {
            cout << "Conexión cerrada por el servidor.\n";
            break;
        } else {
            cerr << "recv() falló: " << WSAGetLastError() << "\n";
            break;
        }
    }

    // 7) Limpieza
    closesocket(connSock);
    WSACleanup();
    return 0;
}
