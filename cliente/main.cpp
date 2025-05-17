// cliente/main.cpp
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501   // Windows XP o superior
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "protocolo.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 2048

int main() {
    WSADATA wsaData;
    int iResult;

    // 1) Inicializar Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup falló: " << iResult << "\n";
        return 1;
    }

    // 2) Resolver dirección del servidor
    struct addrinfo hints{}, *serverInfo = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo(SERVER_ADDRESS, DEFAULT_PORT, &hints, &serverInfo);
    if (iResult != 0) {
        std::cerr << "getaddrinfo falló: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    // 3) Crear socket
    SOCKET connSock = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    if (connSock == INVALID_SOCKET) {
        std::cerr << "socket() falló: " << WSAGetLastError() << "\n";
        freeaddrinfo(serverInfo);
        WSACleanup();
        return 1;
    }

    // 4) Conectar al servidor
    iResult = connect(connSock, serverInfo->ai_addr, (int)serverInfo->ai_addrlen);
    freeaddrinfo(serverInfo);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect() falló: " << WSAGetLastError() << "\n";
        closesocket(connSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Conectado al servidor " << SERVER_ADDRESS << ":" << DEFAULT_PORT << "\n";

    // 5) Menú y bucle de interacción
    bool salir = false;
    char recvbuf[BUFFER_SIZE];
    while (!salir) {
        std::cout << "\n1) Consultar historial\n2) Agregar historial\n3) Salir\n> ";
        int opcion; std::cin >> opcion;

        std::string request;
        switch (opcion) {
            case 1: {
                int id; std::cout << "ID Paciente: "; std::cin >> id;
                request = formatRequest(CMD_CONSULTA_HISTORIAL, { std::to_string(id) });
                break;
            }
            case 2: {
                int id; std::cout << "ID Paciente: "; std::cin >> id;
                // pide más datos según tu Fase 1…
                request = formatRequest(CMD_AGREGAR_HISTORIAL, { std::to_string(id) /*, resto */ });
                break;
            }
            case 3:
                request = formatRequest(CMD_SALIR, {});
                salir = true;
                break;
            default:
                std::cout << "Opción no válida.\n";
                continue;
        }

        // 6) Enviar y recibir
        iResult = send(connSock, request.c_str(), (int)request.size(), 0);
        if (iResult == SOCKET_ERROR) {
            std::cerr << "send() falló: " << WSAGetLastError() << "\n";
            break;
        }

        iResult = recv(connSock, recvbuf, BUFFER_SIZE - 1, 0);
        if (iResult > 0) {
            recvbuf[iResult] = '\0';
            std::cout << "Respuesta: " << recvbuf;
        } else if (iResult == 0) {
            std::cout << "Conexión cerrada por el servidor.\n";
            break;
        } else {
            std::cerr << "recv() falló: " << WSAGetLastError() << "\n";
            break;
        }
    }

    // 7) Limpieza
    closesocket(connSock);
    WSACleanup();
    return 0;
}
