#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hospital.h"
#include "bd/sqlite3.h"

#define BD_PATH "BD_HOSPITAL"

// --------------- Consulta de historial ---------------

char* hospital_consultar_historial(int idPaciente) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT h.id, p.nombre, h.antecedente "
        "FROM HistClinica h "
        "JOIN Paciente p ON h.paciente_id = p.id "
        "WHERE p.id = ?;";

    if (sqlite3_open(BD_PATH, &db) != SQLITE_OK) {
        const char *err = sqlite3_errmsg(db);
        char *msg = malloc(256);
        snprintf(msg, 256, "ERR: No se pudo abrir BD: %s", err);
        sqlite3_close(db);
        return msg;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        const char *err = sqlite3_errmsg(db);
        char *msg = malloc(256);
        snprintf(msg, 256, "ERR preparar consulta: %s", err);
        sqlite3_close(db);
        return msg;
    }

    sqlite3_bind_int(stmt, 1, idPaciente);

    // Buffer dinámico para la respuesta
    size_t cap = 1024;
    size_t len = 0;
    char *res = malloc(cap);
    if (!res) { sqlite3_finalize(stmt); sqlite3_close(db); return strdup("ERR: sin memoria"); }
    res[0] = '\0';

    int filas = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        filas++;
        int idHist = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char*)sqlite3_column_text(stmt, 1);
        const char *ante = (const char*)sqlite3_column_text(stmt, 2);

        // cada línea: "ID|Nombre|Antecedente\n"
        char linea[512];
        int r = snprintf(linea, sizeof(linea), "%d|%s|%s\n", idHist, nombre, ante);
        if (len + r + 1 > cap) {
            cap *= 2;
            res = realloc(res, cap);
            if (!res) { sqlite3_finalize(stmt); sqlite3_close(db); return strdup("ERR: sin memoria"); }
        }
        memcpy(res + len, linea, r);
        len += r;
        res[len] = '\0';
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (filas == 0) {
        free(res);
        return strdup("ERR: No se encontraron historiales para ese paciente\n");
    }
    return res;
}

// --------------- Agregar historial ---------------

int hospital_agregar_historial(int idPaciente, const char *antecedente) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "INSERT INTO HistClinica (paciente_id, antecedente) VALUES (?, ?);";

    if (sqlite3_open(BD_PATH, &db) != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return -2;
    }

    sqlite3_bind_int(stmt, 1, idPaciente);
    sqlite3_bind_text(stmt, 2, antecedente, -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return (step == SQLITE_DONE) ? 0 : -3;
}
