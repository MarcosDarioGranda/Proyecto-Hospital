// servidor/main.cpp
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501   // Windows XP o superior
#endif

#include <winsock2.h>    // Debe incluirse antes de windows.h
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "protocolo.h"
#include "../lib/include/hospital.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define BACKLOG 5
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

    // 2) Preparar hints y resolver dirección
    struct addrinfo hints = {};
    struct addrinfo *addrResult = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;  // Para bind()
    iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &addrResult);
    if (iResult != 0) {
        std::cerr << "getaddrinfo falló: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    // 3) Crear socket para escuchar
    SOCKET listenSock = socket(
        addrResult->ai_family,
        addrResult->ai_socktype,
        addrResult->ai_protocol
    );
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "socket() falló: " << WSAGetLastError() << "\n";
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    // 4) Bind al puerto
    iResult = bind(listenSock, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    freeaddrinfo(addrResult);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind() falló: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    // 5) Listen
    iResult = listen(listenSock, BACKLOG);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen() falló: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Servidor escuchando en puerto " << DEFAULT_PORT << "\n";

    // 6) Aceptar cliente
    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "accept() falló: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Cliente conectado.\n";

    // 7) Bucle de atención al cliente
    char buffer[BUFFER_SIZE];
    while (true) {
        iResult = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::string req(buffer);
            Command cmd = parseCommand(req);
            std::string response;

            switch (cmd) {
                case CMD_CONSULTA_HISTORIAL: {
                    auto fields = splitFields(req);
                    int id = std::stoi(fields[1]);
                    char *datos = hospital_consultar_historial(id);
                    response = "OK|" + std::string(datos);
                    free(datos);
                    break;
                }
                case CMD_AGREGAR_HISTORIAL: {
                    auto fields = splitFields(req);
                    int id = std::stoi(fields[1]);
                    const char *ante = fields[2].c_str();
                    int rc = hospital_agregar_historial(id, ante);
                    response = (rc == 0)
                        ? "OK|Historial agregado\n"
                        : "ERR|No se pudo agregar historial\n";
                    break;
                }
                case CMD_SALIR:
                    response = "OK|Adiós\n";
                    send(clientSock, response.c_str(), (int)response.size(), 0);
                    goto cleanup;
                default:
                    response = "ERR|Comando desconocido\n";
            }
            send(clientSock, response.c_str(), (int)response.size(), 0);
        }
        else if (iResult == 0) {
            std::cout << "Conexión cerrada por el cliente.\n";
            break;
        }
        else {
            std::cerr << "recv() falló: " << WSAGetLastError() << "\n";
            break;
        }
    }

cleanup:
    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
