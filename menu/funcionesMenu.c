#include "funcionesMenu.h"
#include "menus.h"
#include "../bd/sqlite3.h"
#include <stdio.h>
#include <time.h>

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
        menuOpciones();
        
    } else {
        printf("Usuario o contraseña incorrectos\n");
      
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// Función para registrar el inicio de sesión en un archivo
/*void registrarInicioSesion(const char *usuario) {
    FILE *archivo = fopen("logins.txt", "a"); // Abre en modo "append"
    if (archivo == NULL) {
        printf("Error al abrir el archivo de registros.\n");
        return;
    }

    // Obtener la fecha y hora actual
    time_t t;
    struct tm *tm_info;
    char buffer[30];

    time(&t);
    tm_info = localtime(&t);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);

    // Escribir en el archivo
    fprintf(archivo, "Usuario: %s | Hora: %s\n", usuario, buffer);
    fclose(archivo);
}*/

