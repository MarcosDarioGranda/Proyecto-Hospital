#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "../lib/src/bd/sqlite3.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "6000"
#define BUFFER_SIZE 2048
#define DB_PATH "data/hospital.db"

using namespace std;

sqlite3* abrirBaseDeDatos() {
    sqlite3* db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        cerr << "Error abriendo la base de datos: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
}

string listarPacientes() {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    string query = "SELECT id, nombre, fecha_nac, direccion, tlfn FROM paciente;";
    sqlite3_stmt* stmt;
    string resultado;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            resultado += to_string(sqlite3_column_int(stmt, 0)) + ". ";
            resultado += (const char*)sqlite3_column_text(stmt, 1);
            resultado += " | " + string((const char*)sqlite3_column_text(stmt, 2));
            resultado += " | " + string((const char*)sqlite3_column_text(stmt, 3));
            resultado += " | " + to_string(sqlite3_column_int(stmt, 4)) + "\n";
        }
    } else {
        resultado = "Error en consulta SELECT.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultado.empty() ? "No hay pacientes.\n" : resultado;
}

string buscarPacientePorID(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    string query = "SELECT id, nombre, fecha_nac, direccion, tlfn FROM paciente WHERE id = ?;";
    sqlite3_stmt* stmt;
    string resultado;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, stoi(id_str));
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            resultado += to_string(sqlite3_column_int(stmt, 0)) + ". ";
            resultado += (const char*)sqlite3_column_text(stmt, 1);
            resultado += " | " + string((const char*)sqlite3_column_text(stmt, 2));
            resultado += " | " + string((const char*)sqlite3_column_text(stmt, 3));
            resultado += " | " + to_string(sqlite3_column_int(stmt, 4)) + "\n";
        } else {
            resultado = "Paciente no encontrado.\n";
        }
    } else {
        resultado = "Error en consulta SELECT.";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return resultado;
}

string anyadirPaciente(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    stringstream ss(datos);
    string nombre, fecha, direccion, tlfn;
    getline(ss, nombre, ',');
    getline(ss, fecha, ',');
    getline(ss, direccion, ',');
    getline(ss, tlfn);

    string query = "INSERT INTO paciente (nombre, fecha_nac, direccion, tlfn) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, fecha.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, direccion.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, stoi(tlfn));

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Paciente añadido con éxito.\n";
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

    string query = "UPDATE paciente SET nombre=?, fecha_nac=?, direccion=?, tlfn=? WHERE id=?;";
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

string procesarComando(const string& entrada) {
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


