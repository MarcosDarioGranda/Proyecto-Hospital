#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "../lib/src/bd/sqlite3.h"
#include "../clases/Paciente.h"
#include "../clases/Persona.h"
#include "funciones_pacientes.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define BUFFER_SIZE 2048
#define DB_PATH "BD_HOSPITAL"

using namespace std;

static sqlite3* abrirBaseDeDatos() {
    sqlite3* db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        cerr << "Error abriendo la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

//implementada con oo
string listarPacientes() {
    sqlite3* db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        return "Error al abrir la base de datos.";
    }

    const char* query = "SELECT id, nombre, fecha_nac, dir, TF FROM paciente;";
    sqlite3_stmt* stmt;
    vector<Paciente*> pacientes;
    ostringstream resultado;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string fecha = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string direccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int telefono = sqlite3_column_int(stmt, 4);

            Paciente* p = new Paciente(id, nombre, fecha, direccion, telefono);
            pacientes.push_back(p);
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    for (const auto& p : pacientes) {
        resultado << static_cast<Paciente*>(p)->toString() << "\n";
        delete p; // liberar memoria
    }

    return resultado.str();
}
//implementada con oo
string buscarPacientePorID(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    string query = "SELECT id, nombre, fecha_nac, dir, TF FROM paciente WHERE id = ?;";
    sqlite3_stmt* stmt;
    string resultado;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // Convertir el id a int y enlazar
        sqlite3_bind_int(stmt, 1, stoi(id_str));

        // Si se encuentra un resultado
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string fechaNacimiento = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            string direccion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int telefono = sqlite3_column_int(stmt, 4);

            // Crear objeto Paciente
            Paciente paciente(id, nombre, fechaNacimiento, direccion, telefono);
            resultado = paciente.toString();
        } else {
            resultado = "Paciente no encontrado.\n";
        }
    } else {
        resultado = "Error en consulta SELECT.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultado;
}

//implementada con oo
string anyadirPaciente(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    Paciente paciente = Paciente::fromCSV(datos);

    string query = "INSERT INTO paciente (nombre, fecha_nac, dir, TF) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, paciente.getNombre().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, paciente.getFechaNacimiento().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, paciente.getDireccion().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, paciente.getTelefono());

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Paciente anyadido con exito.\n";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al añadir paciente.\n";
}

string modificarPaciente(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    stringstream ss(datos);
    string id, nombre, fecha, direccion, tlfn;
    getline(ss, id, ',');
    getline(ss, nombre, ',');
    getline(ss, fecha, ',');
    getline(ss, direccion, ',');
    getline(ss, tlfn);

    string query = "UPDATE paciente SET nombre=?, fecha_nac=?, dir=?, TF=? WHERE id=?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, fecha.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, direccion.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, stoi(tlfn));
        sqlite3_bind_int(stmt, 5, stoi(id));

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Paciente modificado.\n";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al modificar paciente.\n";
}

string eliminarPaciente(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    string query = "DELETE FROM paciente WHERE id=?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, stoi(id_str));
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Paciente eliminado.\n";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al eliminar paciente.\n";
}

string procesarComandoPacientes(const string& entrada) {
    istringstream iss(entrada);
    string comando;
    iss >> comando;
    string argumentos;
    getline(iss, argumentos);
    if (!argumentos.empty() && argumentos[0] == ' ') argumentos.erase(0, 1);

    if (comando == "LISTAR_PACIENTES") return listarPacientes();
    if (comando == "BUSCAR_PACIENTE") return buscarPacientePorID(argumentos);
    if (comando == "AÑADIR_PACIENTE") return anyadirPaciente(argumentos);
    if (comando == "MODIFICAR_PACIENTE") return modificarPaciente(argumentos);
    if (comando == "ELIMINAR_PACIENTE") return eliminarPaciente(argumentos);
    if (comando == "SALIR") return "Desconectando...\n";

    return "Comando no reconocido\n";
}

void menuPacientes() {
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo hints, *result = NULL;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    //getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    listen(ListenSocket, SOMAXCONN);

    cout << "Servidor esperando conexiones en el puerto " << DEFAULT_PORT << "...\n";

    ClientSocket = accept(ListenSocket, NULL, NULL);
    cout << "Cliente conectado.\n";

    char recvbuf[BUFFER_SIZE];
    int recvbuflen = BUFFER_SIZE;

    while (true) {
        memset(recvbuf, 0, recvbuflen);
        int bytesRecibidos = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (bytesRecibidos <= 0) break;

        string comando(recvbuf);
        string respuesta = procesarComandoPacientes(comando);
        send(ClientSocket, respuesta.c_str(), (int)respuesta.length(), 0);

        if (comando.find("SALIR") == 0) break;
    }

    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();

    cout << "Servidor cerrado.\n";
    
}
