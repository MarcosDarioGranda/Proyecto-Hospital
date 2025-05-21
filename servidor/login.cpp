#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

bool validarLogin(const string& nombre, const string& contrasenya, string& rol_out) {
    sqlite3* db;
    if (sqlite3_open("data/hospital.db", &db) != SQLITE_OK) {
        cerr << "No se pudo abrir la base de datos\n";
        return false;
    }

    string query = "SELECT rol FROM usuario WHERE nombre = ? AND contrasenya = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasenya.c_str(), -1, SQLITE_STATIC);

    bool valid = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* rol = sqlite3_column_text(stmt, 0);
        rol_out = string(reinterpret_cast<const char*>(rol));
        valid = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return valid;
}
