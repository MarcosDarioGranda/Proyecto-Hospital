#include <stdio.h>     // Biblioteca estándar para entrada y salida
#include "sqlite3.h"   // Biblioteca de SQLite para manejar bases de datos en C
#include <string.h>    // Biblioteca para manejo de cadenas de caracteres

int main() {
    sqlite3 *db;           // Puntero a la base de datos
    sqlite3_stmt *stmt;    // Puntero a una declaración SQL preparada
    int result;            // Variable para almacenar resultados de las operaciones

    // Abre la base de datos "BD_HOSPITAL.sqlite"
    sqlite3_open("BD_HOSPITAL.sqlite", &db);

    /* --- INSERT --- */
    // Consulta SQL para insertar datos en la tabla "campeonatos"
    char sql1[] = "insert into campeonatos (id, nombre, campeon) values (NULL, ?, ?)";
    char nombre[] = "Copa 2020-21";  // Nombre del campeonato a insertar
    int campeon = 3;  // ID del equipo campeón

    // Prepara la consulta SQL para su ejecución
    sqlite3_prepare_v2(db, sql1, strlen(sql1) + 1, &stmt, NULL);

    // Asigna los valores a los parámetros de la consulta
    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);  // Asigna el nombre del campeonato
    sqlite3_bind_int(stmt, 2, campeon);  // Asigna el ID del equipo campeón

    // Ejecuta la consulta
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {  // Verifica si hubo un error
        printf("Error insertando el campeonato\n");
    } else {
        printf("Campeonato %s(%i) insertado\n", nombre, campeon);
    }

    // Libera la memoria utilizada por la consulta preparada
    sqlite3_finalize(stmt);
    /* --- INSERT (fin) --- */

    /* --- SELECT --- */
    // Consulta SQL para seleccionar campeonatos donde el equipo ganador es un equipo específico
    char sql2[] = "select c.nombre from campeonatos c, equipos e where c.campeon=e.id and e.nombre=?";
    char equipo[] = "F.C. Barcelona";  // Nombre del equipo a buscar

    // Prepara la consulta SQL
    sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL);
    sqlite3_bind_text(stmt, 1, equipo, strlen(equipo), SQLITE_STATIC);  // Asigna el valor del equipo

    printf("\n");
    printf("Mostrando campeonatos de %s:\n", equipo);

    // Ejecuta la consulta y muestra los resultados
    do {
        result = sqlite3_step(stmt);
        if (result == SQLITE_ROW) {  // Si hay una fila de resultados
            printf("%s\n", (char*) sqlite3_column_text(stmt, 0));  // Imprime el nombre del campeonato
        }
    } while (result == SQLITE_ROW);  // Repite hasta que no haya más filas

    printf("\n");

    // Libera la memoria de la consulta preparada
    sqlite3_finalize(stmt);

    /* --- SELECT (fin) --- */

    // Cierra la conexión con la base de datos
    sqlite3_close(db);

    return 0;
}