#include "funcionesMenu.h"
#include "menus.h"
#include "../bd/sqlite3.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include <time.h>
#include "../entidades/paciente.h"
#include <string.h>

//Para ejecutar cada consulta/modificación
void ejecutarSQL(sqlite3* db, const char* sql) {
    char *errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        printf("Error SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

//Iniciar Sesión
void iniciarSesion() {
    sqlite3* db;
    if (sqlite3_open("BD_HOSPITAL", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    char usuario[50], contrasena[50];
    printf("Usuario: ");
    scanf("%49s", usuario);
    printf("Contraseña: ");
    scanf("%49s", contrasena);

    const char* sql = "SELECT COUNT(*) FROM Medico WHERE nombre = ? AND contraseña = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Error preparando la consulta\n");
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(stmt, 1, usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
        printf("Inicio de sesión exitoso\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        menuOpciones();
        return;
        
        
        
    } else {
        printf("Usuario o contraseña incorrectos\n");
        menuInicioSesion();

    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

///////////////////////////////////////////////////////////////////////////////
//Funciones de Gestion de pacientes
void agregarPaciente(sqlite3 *db) {
    char nombre[21];
    char fecha_nac[11];
    char dir[31];
    int tf;

    printf("Nombre (max 20 caracteres): ");
    scanf(" %[^\n]", nombre);
    printf("Fecha de nacimiento (YYYY-MM-DD): ");
    scanf(" %[^\n]", fecha_nac);
    printf("Dirección (max 30 caracteres): ");
    scanf(" %[^\n]", dir);
    printf("Teléfono fijo (9 dígitos): ");
    scanf("%d", &tf);

    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Paciente (nombre, fecha_nac, dir, TF) VALUES ('%s', '%s', '%s', %d);",
             nombre, fecha_nac, dir, tf);

    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error al insertar paciente: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Paciente agregado correctamente.\n");
    }
}

void buscarPaciente(sqlite3 *db) {
    int id;
    printf("Ingrese el ID del paciente: ");
    scanf("%d", &id);

    const char *sql = "SELECT id, nombre, fecha_nac, dir, TF FROM Paciente WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("\n--- Datos del Paciente ---\n");
        //Hacer con una funcion de imprimir Paciente
        Paciente* p = (Paciente*) malloc(sizeof(Paciente));
        p->id = sqlite3_column_int(stmt, 0);
        strcpy(p->nombre,sqlite3_column_text(stmt, 1));
        strcpy(p->fecha_nac, sqlite3_column_text(stmt, 2));
        strcpy(p->direccion, sqlite3_column_text(stmt, 3));
        p->tlfn = sqlite3_column_int(stmt, 4);
        imprimirPaciente(*p);
        free(p);
    } else {
        printf("No se encontró ningún paciente con ID %d.\n", id);
    }

    sqlite3_finalize(stmt);
}

void modificarPaciente(sqlite3 *db) {
    int id;
    printf("Ingrese el ID del paciente a modificar: ");
    scanf("%d", &id);

    // Comprobar si existe
    const char *sql_select = "SELECT nombre, fecha_nac, dir, TF FROM Paciente WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar consulta SELECT: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("No se encontró ningún paciente con ID %d.\n", id);
        sqlite3_finalize(stmt);
        return;
    }

    // Nuevos datos
    char nuevo_nombre[21];
    char nueva_fecha[11];
    char nueva_dir[31];
    int nuevo_tf;

    printf("\n--- Ingrese los nuevos datos ---\n");
    printf("Nuevo Nombre (max 20 caracteres): ");
    scanf(" %[^\n]", nuevo_nombre);
    printf("Nueva Fecha de nacimiento (YYYY-MM-DD): ");
    scanf(" %[^\n]", nueva_fecha);
    printf("Nueva Dirección (max 30 caracteres): ");
    scanf(" %[^\n]", nueva_dir);
    printf("Nuevo Teléfono: ");
    scanf("%d", &nuevo_tf);

    // Actualizar
    const char *sql_update = "UPDATE Paciente SET nombre = ?, fecha_nac = ?, dir = ?, TF = ? WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar UPDATE: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nuevo_nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nueva_fecha, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, nueva_dir, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, nuevo_tf);
    sqlite3_bind_int(stmt, 5, id);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Paciente actualizado correctamente.\n");
    } else {
        printf("Error al actualizar paciente: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void eliminarPaciente(sqlite3 *db) {
    int id;
    printf("Ingrese el ID del paciente a eliminar: ");
    scanf("%d", &id);

    // Comprobamos si existe
    const char *sql_check = "SELECT nombre FROM Paciente WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("No se encontró ningún paciente con ID %d.\n", id);
        sqlite3_finalize(stmt);
        return;
    }

    char *nombre;
    strcpy(nombre, sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);

    printf("¿Está seguro que desea eliminar al paciente '%s'? (s/n): ", nombre);
    char confirm;
    scanf(" %c", &confirm);

    if (confirm != 's' && confirm != 'S') {
        printf("Operación cancelada.\n");
        return;
    }

    const char *sql_delete = "DELETE FROM Paciente WHERE id = ?;";
    if (sqlite3_prepare_v2(db, sql_delete, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando DELETE: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Paciente eliminado correctamente.\n");
    } else {
        printf("Error al eliminar paciente: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}
