
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
#include "../clases/HistorialMedico.h"
#include "funciones_historiales.h"
#include <algorithm>  
#include <cctype>
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

string consultarHistorialDelPaciente(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.";

    string sql =
        "SELECT h.id, h.paciente_id, p.nombre, h.antecedente "
        "FROM HistClinica h "
        "JOIN Paciente p ON h.paciente_id = p.id "
        "WHERE h.paciente_id = ?";

    sqlite3_stmt* stmt;
    string resultado;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, id_str.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* paciente_id = sqlite3_column_text(stmt, 1);
            const unsigned char* nombre = sqlite3_column_text(stmt, 2);
            const unsigned char* antecedente = sqlite3_column_text(stmt, 3);

            resultado += "ID: " + to_string(id) +
                         ", Paciente ID: " + string(reinterpret_cast<const char*>(paciente_id)) +
                         ", Nombre: " + string(reinterpret_cast<const char*>(nombre)) +
                         ", Antecedente: " + string(reinterpret_cast<const char*>(antecedente)) + "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        resultado = "Error al preparar la consulta.";
    }

    sqlite3_close(db);
    return resultado.empty() ? "No se encontraron historiales." : resultado;
}


string agregarHistorial(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    // 1. Parsear la línea CSV → objeto HistorialMedico
    HistorialMedico historial = HistorialMedico::fromCSV(datos);

    // 2. Sentencia SQL
    string query =
        "INSERT INTO HistClinica (paciente_id, antecedente) "
        "VALUES (?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, historial.getIdPaciente());
        sqlite3_bind_text(stmt, 2, historial.getAntecedente().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Historial anyadido con exito.\n";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al añadir historial. No se ha conectado correctamente, o el paciente no existe.\n";
}

string modificarHistorial(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    // 1. Parsear línea CSV → objeto HistorialMedico
    HistorialMedico historial = HistorialMedico::fromCSV(datos);

    // 2. Sentencia SQL: actualiza TODOS los campos
    string query =
        "UPDATE HistClinica "
        "SET paciente_id = ?, antecedente = ? "
        "WHERE id = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, historial.getIdPaciente());
        sqlite3_bind_text(stmt, 2, historial.getAntecedente().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, historial.getId());     // id del registro a modificar

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            // ¿Se modificó realmente una fila?
            if (sqlite3_changes(db) > 0) {
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return "Historial modificado con exito.\n";
            } else {
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return "No se encontró el historial especificado.\n";
            }
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al modificar historial.\n";
}


string eliminarHistorial(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    // Validar que el ID sea numérico
    if (id_str.empty() || !std::all_of(id_str.begin(), id_str.end(), [](unsigned char c) { return std::isdigit(c); })) {
        sqlite3_close(db);
        return "ID inválido. Debe ser un número.\n";
    }

    int id = stoi(id_str);

    string query = "DELETE FROM HistClinica WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            int filasAfectadas = sqlite3_changes(db);
            sqlite3_finalize(stmt);
            sqlite3_close(db);

            if (filasAfectadas > 0) {
                return "Historial eliminado con éxito.\n";
            } else {
                return "No se encontró un historial con ese ID.\n";
            }
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al eliminar historial.\n";
}
string procesarComandoHistoriales(const string& entrada) {
    istringstream iss(entrada);
    string comando;
    iss >> comando;
    string argumentos;
    getline(iss, argumentos);
    if (!argumentos.empty() && argumentos[0]==' ') argumentos.erase(0,1);

    if (comando == "CONSULTAR_HISTORIAL") return consultarHistorialDelPaciente(argumentos);
    if (comando == "ANADIR_HISTORIAL")      return agregarHistorial(argumentos);
    if (comando == "MODIFICAR_HISTORIAL")  return modificarHistorial(argumentos);
    if (comando == "ELIMINAR_HISTORIAL")   return eliminarHistorial(argumentos);
    if (comando == "SALIR")                return "Desconectando...\n";
    return "Comando no reconocido\n";
}

