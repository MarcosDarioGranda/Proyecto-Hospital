#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "protocolo.h"
#include "../lib/include/hospital.h"
#include "login.h"

#include "../funciones/funciones_pacientes.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define BACKLOG 5
#define BUFFER_SIZE 2048

int main() {
std::ofstream srvLog("server.log", std::ios::app);
    if (!srvLog.is_open()) {
        std::cerr << "ERROR: no puedo abrir server.log para escribir\n";
        return 1;
    }
    srvLog << "[INIT] Servidor arrancado\n";

    // -- INICIALIZAR WINSOCK --
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup fallo: " << iResult << "\n";
        return 1;
    }

    // Ahora ya puedes usar getaddrinfo, socket, bind, listen, etc.
    struct addrinfo hints = {};
    struct addrinfo *addrResult = nullptr;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &addrResult);
    if (iResult != 0) {
        std::cerr << "getaddrinfo fallo: " << iResult << "\n";
        WSACleanup();
        return 1;
    }

    SOCKET listenSock = socket(
        addrResult->ai_family,
        addrResult->ai_socktype,
        addrResult->ai_protocol
    );
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "socket() fallo: " << WSAGetLastError() << "\n";
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    iResult = bind(listenSock, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    freeaddrinfo(addrResult);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind() fallo: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    iResult = listen(listenSock, BACKLOG);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen() fallo: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Servidor escuchando en puerto " << DEFAULT_PORT << "\n";

    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "accept() falló: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Cliente conectado.\n";

    char buffer[BUFFER_SIZE];
    while (true) {
        iResult = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::string req(buffer);
            while (!req.empty() && (req.back() == '\r' || req.back() == '\n'))
            req.pop_back(); 
            srvLog << "[REQ] " << req << "\n";
            srvLog.flush();
            std::string response;
            // --- DEBUGGING LOGIN ---
            if (req.rfind("LOGIN", 0) == 0) {
                auto f = splitFields(req);
                std::cerr << "[DEBUG] raw req='" << req << "'\n";
                std::cerr << "[DEBUG] campos recibidos: " << f.size() << "\n";
                for (size_t i = 0; i < f.size(); ++i) {
                    std::cerr << "  campo[" << i << "]='" << f[i] << "'\n";
                }

                std::string response;
                if (f.size() < 3) {
                    response = "ERR|Faltan parametros\n";
                }else {
                    std::string rol;
                    if (validarLogin(f[1], f[2], rol)) {
                        response = "OK|" + rol + "\n";
                    } else {
                        response = "ERR|Usuario o contrasenya incorrectos\n";
                    }
                }
                send(clientSock, response.c_str(), response.size(), 0);
                continue;
            }
            

            Command cmd = parseCommand(req);
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
                    auto f = splitFields(req);
                    if (f.size() < 3) {
                        response = "ERR|Parametros insuficientes para AGREGAR_HISTORIAL\n";
                        break;
                    }
                    int id = std::stoi(f[1]);
                    const char *ante = f[2].c_str();
                    int rc = hospital_agregar_historial(id, ante);
                    response = (rc == 0)
                        ? "OK|Historial agregado\n"
                        : "ERR|No se pudo agregar historial\n";
                    break;
                }
                case CMD_SALIR:
                    response = "OK|Adios\n";
                    send(clientSock, response.c_str(), (int)response.size(), 0);
                    goto cleanup;
                default:
                    response = procesarComando(req);
            }

            srvLog << "[RES] " << response;
            srvLog.flush();
            send(clientSock, response.c_str(), (int)response.size(), 0);
        }
        else if (iResult == 0) {
            std::cout << "Conexion cerrada por el cliente.\n";
            break;
        }
        else {
            std::cerr << "recv() fallo: " << WSAGetLastError() << "\n";
            break;
        }
    }

cleanup:
    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
