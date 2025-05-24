#include <iostream>
#include "../lib/src/bd/sqlite3.h"
#include <string>

using namespace std;

#define DB_PATH "BD_HOSPITAL"

bool validarLogin(const string& nombre, const string& contrasenya, string& rol_out) {
    sqlite3* db;
    // Intento de apertura y log de ruta
    cerr << "[DEBUG-login] Abriendo BD en: " << DB_PATH << "\n";
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        cerr << "[DEBUG-login] No se pudo abrir la base de datos: "
             << sqlite3_errmsg(db) << "\n";
        return false;
    }

    // Asegúrate de usar el nombre de tabla y columnas que existen en tu BD:
    const char* sql = 
        "SELECT rol "
        "FROM Medico "
        "WHERE nombre = ? AND contraseña = ?;";
    cerr << "[DEBUG-login] SQL: " << sql << "\n";
    cerr << "[DEBUG-login] Parametros: nombre='" << nombre
         << "', contraseña='" << contrasenya << "'\n";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "[DEBUG-login] prepare error: " 
             << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, nombre.c_str(),    -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasenya.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    cerr << "[DEBUG-login] sqlite3_step returned: " << step << "\n";

    bool valid = false;
    if (step == SQLITE_ROW) {
        const unsigned char* rol = sqlite3_column_text(stmt, 0);
        rol_out = reinterpret_cast<const char*>(rol);
        cerr << "[DEBUG-login] rol recuperado: '" << rol_out << "'\n";
        valid = true;
    } else if (step == SQLITE_DONE) {
        cerr << "[DEBUG-login] usuario o clave no encontrados\n";
    } else {
        cerr << "[DEBUG-login] step error: " 
             << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return valid;
}
