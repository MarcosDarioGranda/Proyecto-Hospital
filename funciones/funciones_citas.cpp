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
#include "../clases/Cita.h"
#include "../clases/Medico.h"
#include "funciones_citas.h"
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

bool verificarPacienteExiste(sqlite3* db, int idPaciente) {
    if (idPaciente <= 0) return false;                    // Validación rápida

    const char* sql = "SELECT 1 FROM Paciente WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, idPaciente);
    bool existe = (sqlite3_step(stmt) == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return existe;
}

bool verificarMedicoExiste(sqlite3 *db, int idMedico) {
    const char* sql = "SELECT 1 FROM Medico WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return "false";

    sqlite3_bind_int(stmt, 1, idMedico);
    bool existe = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return existe ? "true" : "false";
}


string consultarCitasPorPaciente(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    if (id_str.empty() || !all_of(id_str.begin(), id_str.end(),
        [](unsigned char c){ return isdigit(c); })) {
        sqlite3_close(db);
        return "ID de paciente inválido.\n";
    }

    int idPaciente = stoi(id_str);

    // ► Verificar existencia del paciente
    if (!verificarPacienteExiste(db, idPaciente)) {
        sqlite3_close(db);
        return "Paciente no encontrado.\n";
    }

    const char* sql =
        "SELECT id, paciente_id, fecha, medico_id, estado "
        "FROM Cita WHERE paciente_id = ?;";

    sqlite3_stmt* stmt;
    string result;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, idPaciente);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result += "ID: "      + to_string(sqlite3_column_int(stmt, 0)) +
                      ", Paciente: " + to_string(sqlite3_column_int(stmt, 1)) +
                      ", Fecha: "  + string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) +
                      ", Médico: " + to_string(sqlite3_column_int(stmt, 3)) +
                      ", Estado: " + string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) +
                      "\n";
        }
    } else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "Error al preparar la consulta.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result.empty() ? "No hay citas para este paciente.\n" : result;
}

string consultarCitasPorMedico(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    if (id_str.empty() || !all_of(id_str.begin(), id_str.end(),
        [](unsigned char c){ return isdigit(c); })) {
        sqlite3_close(db);
        return "ID de médico inválido.\n";
    }

    int idMedico = stoi(id_str);

    // ► Verificar existencia del médico
    if (!verificarMedicoExiste(db, idMedico)) {
        sqlite3_close(db);
        return "Médico no encontrado.\n";
    }

    const char* sql =
        "SELECT id, paciente_id, fecha, medico_id, estado "
        "FROM Cita WHERE medico_id = ?;";

    sqlite3_stmt* stmt;
    string result;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, idMedico);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result += "ID: "      + to_string(sqlite3_column_int(stmt, 0)) +
                      ", Paciente: " + to_string(sqlite3_column_int(stmt, 1)) +
                      ", Fecha: "  + string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))) +
                      ", Médico: " + to_string(sqlite3_column_int(stmt, 3)) +
                      ", Estado: " + string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))) +
                      "\n";
        }
    } else {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "Error al preparar la consulta.\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result.empty() ? "No hay citas para este médico.\n" : result;
}


string agregarCita(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    Cita cita = Cita::fromCSV(datos);

    // Verificar existencia de paciente y médico
    if (!verificarPacienteExiste(db, cita.getIdPaciente())) {
        sqlite3_close(db);
        return "Error: el paciente no existe.\n";
    }
    if (!verificarMedicoExiste(db, cita.getIdMedico())) {
        sqlite3_close(db);
        return "Error: el médico no existe.\n";
    }

    const char* sql =
        "INSERT INTO Cita (paciente_id, fecha, medico_id, estado) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int (stmt, 1, cita.getIdPaciente());
        sqlite3_bind_text(stmt, 2, cita.getFechaHora().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int (stmt, 3, cita.getIdMedico());
        sqlite3_bind_text(stmt, 4, cita.getEstado().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return "Cita agregada con éxito.\n";
        }
    }

    string err = sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return "Error al agregar la cita: " + err + "\n";
}



string modificarCita(const string& datos) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    //Parsear CSV 
    stringstream ss(datos);
    string idStr, fecha, estado, medicoStr;
    getline(ss, idStr,   ',');
    getline(ss, fecha,   ',');
    getline(ss, estado,  ',');
    getline(ss, medicoStr, ',');        // puede quedar vacío

    if (idStr.empty() || fecha.empty() || estado.empty() ||
        !all_of(idStr.begin(), idStr.end(), ::isdigit)) {
        sqlite3_close(db);
        return "Formato incorrecto.\n";
    }

    int idCita   = stoi(idStr);
    bool cambiarMedico = !medicoStr.empty();
    int idMedico = cambiarMedico ? stoi(medicoStr) : 0;

    //Ver si la cita existe
    const char* sqlFind =
        "SELECT id, paciente_id, fecha, medico_id, estado "
        "FROM Cita WHERE id = ?;";
    sqlite3_stmt* stmtFind;

    if (sqlite3_prepare_v2(db, sqlFind, -1, &stmtFind, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return "Error al preparar la verificación.\n";
    }
    sqlite3_bind_int(stmtFind, 1, idCita);
    bool existe = (sqlite3_step(stmtFind) == SQLITE_ROW);
    sqlite3_finalize(stmtFind);

    if (!existe) {
        sqlite3_close(db);
        return "La cita no existe.\n";
    }

    //si se detecta que hay cambio de médico, se cambia también
    if (cambiarMedico && !verificarMedicoExiste(db, idMedico)) {
        sqlite3_close(db);
        return "El nuevo médico no existe.\n";
    }

    //se hace el slq, se añade el medico si este tambien esta en los datos pasados
    string sqlUpdate =
        "UPDATE Cita SET fecha = ?, estado = ?";
    if (cambiarMedico) sqlUpdate += ", medico_id = ?";
    sqlUpdate += " WHERE id = ?;";

    sqlite3_stmt* stmtUp;
    if (sqlite3_prepare_v2(db, sqlUpdate.c_str(), -1, &stmtUp, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return "Error al preparar la actualización.\n";
    }

    // Bind de parámetros
    int idx = 1;
    sqlite3_bind_text(stmtUp, idx++, fecha.c_str(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtUp, idx++, estado.c_str(), -1, SQLITE_STATIC);
    if (cambiarMedico) sqlite3_bind_int(stmtUp, idx++, idMedico);
    sqlite3_bind_int(stmtUp, idx, idCita);  // último parámetro: id

    //Ejecución
    string respuesta;
    if (sqlite3_step(stmtUp) == SQLITE_DONE) {
        respuesta = sqlite3_changes(db) > 0
                  ? "Cita modificada con éxito.\n"
                  : "No se realizaron cambios.\n";
    } else {
        respuesta = string("Error al modificar la cita: ") + sqlite3_errmsg(db) + "\n";
    }

    sqlite3_finalize(stmtUp);
    sqlite3_close(db);
    return respuesta;
}


string eliminarCita(const string& id_str) {
    sqlite3* db = abrirBaseDeDatos();
    if (!db) return "Error al abrir la base de datos.\n";

    // Validación de entrada
    if (id_str.empty() || !all_of(id_str.begin(), id_str.end(), ::isdigit)) {
        sqlite3_close(db);
        return "ID de cita inválido.\n";
    }

    int id = stoi(id_str);

    // Ver si existe
    const char* sqlVerificar = "SELECT id FROM Cita WHERE id = ?;";
    sqlite3_stmt* stmtVerificar;

    if (sqlite3_prepare_v2(db, sqlVerificar, -1, &stmtVerificar, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return "Error al preparar la verificación.\n";
    }

    sqlite3_bind_int(stmtVerificar, 1, id);
    bool existe = (sqlite3_step(stmtVerificar) == SQLITE_ROW);
    sqlite3_finalize(stmtVerificar);

    if (!existe) {
        sqlite3_close(db);
        return "La cita no existe.\n";
    }

    // Eliminacion de cita
    const char* sqlEliminar = "DELETE FROM Cita WHERE id = ?;";
    sqlite3_stmt* stmtEliminar;

    if (sqlite3_prepare_v2(db, sqlEliminar, -1, &stmtEliminar, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return "Error al preparar la eliminación.\n";
    }

    sqlite3_bind_int(stmtEliminar, 1, id);
    string resultado;

    if (sqlite3_step(stmtEliminar) == SQLITE_DONE) {
        resultado = "Cita eliminada con éxito.\n";
    } else {
        resultado = string("Error al eliminar la cita: ") + sqlite3_errmsg(db) + "\n";
    }

    sqlite3_finalize(stmtEliminar);
    sqlite3_close(db);
    return resultado;
}
string procesarComandoCitas(const string& entrada) {
    istringstream iss(entrada);
    string comando; 
    iss >> comando;
    string argumentos;
    getline(iss, argumentos);
    if (!argumentos.empty() && argumentos[0]==' ') argumentos.erase(0,1);

    if (comando == "CONSULTAR_CITAS_PACIENTE") return consultarCitasPorPaciente(argumentos);
    if (comando == "CONSULTAR_CITAS_MEDICO")  return consultarCitasPorMedico(argumentos);
    if (comando == "ANADIR_CITA")             return agregarCita(argumentos);
    if (comando == "MODIFICAR_CITA")          return modificarCita(argumentos);
    if (comando == "ELIMINAR_CITA")           return eliminarCita(argumentos);
    if (comando == "SALIR")                   return "Desconectando...\n";
    return "Comando no reconocido\n";
}
