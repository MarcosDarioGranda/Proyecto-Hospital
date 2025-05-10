#include "funcionesMenu.h"
#include "menus.h"
#include "../bd/sqlite3.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include <time.h>
#include "../entidades/paciente.h"
#include "../entidades/cita.h"
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

//Método que busca todas las citas que un paciente tiene. Se escribe un ID/DNI de paciente, y se devuelve una lista de todas las citas 
//que tiene un paciente.
void consultarCitasPorPaciente(sqlite3 *db){
    int id;
    printf("Ingrese el ID del paciente de el que mostrar las citas:\n");
    scanf("%d", &id);

    const char* sql = "SELECT id, paciente_id, fecha, medico_id, estado FROM Cita WHERE paciente_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    int encontrado = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cita cita;
        cita.idCita = sqlite3_column_int(stmt, 0);
        cita.idPaciente = sqlite3_column_int(stmt, 1);
        strncpy(cita.fecha, (const char*)sqlite3_column_text(stmt, 2), sizeof(cita.fecha));
        cita.fecha[10] = '\0';  // aseguramos terminación
        cita.idMedico = sqlite3_column_int(stmt, 3);
        strncpy(cita.estado, (const char*)sqlite3_column_text(stmt, 4), sizeof(cita.estado));
        cita.estado[sizeof(cita.estado) - 1] = '\0';

        imprimirCita(cita);
        encontrado = 1;
    }

   

    if (!encontrado) {
        printf("No se encontraron citas para el paciente con ID %d.\n", id);
    }
    printf("\nPresione Enter para volver al menú...\n");
    getchar(); // Espera que el usuario presione Enter
    sqlite3_finalize(stmt);
}

//Función que consulta las citas que tiene un Médico. Se escribe el ID, devuelve una lista con todas las citas en
//las que participa
void consultarCitasPorMedico(sqlite3 *db) {
    int id;
    printf("Ingrese el ID del medico del que quiere ver las citas:\n");
    scanf("%d", &id);

    const char* sql = "SELECT id, paciente_id, fecha, medico_id, estado FROM Cita WHERE medico_id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    int encontrado = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Cita cita;
        cita.idCita = sqlite3_column_int(stmt, 0);
        cita.idPaciente = sqlite3_column_int(stmt, 1);
        strncpy(cita.fecha, (const char*)sqlite3_column_text(stmt, 2), sizeof(cita.fecha));
        cita.fecha[sizeof(cita.fecha) - 1] = '\0';
        cita.idMedico = sqlite3_column_int(stmt, 3);
        strncpy(cita.estado, (const char*)sqlite3_column_text(stmt, 4), sizeof(cita.estado));
        cita.estado[sizeof(cita.estado) - 1] = '\0';

        imprimirCitaMedico(cita);
        encontrado = 1;
    }

    

    if (!encontrado) {
        printf("No se encontraron citas para el medico con ID %d.\n", id);
    }
    printf("\nPresione Enter para volver al menú...\n");
    getchar(); // Espera que el usuario presione Enter
    sqlite3_finalize(stmt);
}


//Función utilizada para verificar si un paciente existe. Se utiliza en agregarcita() para a la hora de agregar una cita,
//asegurarse de que el paciente exista (su ID) (Obviamente no puedes pedir una cita para un paciente que no existe)

int verificarPacienteExiste(sqlite3 *db, int idPaciente) {
    const char *sql = "SELECT id FROM Paciente;";
    sqlite3_stmt *stmt;
    int existe = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando verificación de paciente: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        if (id == idPaciente) {
            existe = 1;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return existe;
}
//lo mismo pero para un medico
int verificarMedicoExiste(sqlite3 *db, int idMedico) {
    const char *sql = "SELECT id FROM Medico;";
    sqlite3_stmt *stmt;
    int existe = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando verificación de médico: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        if (id == idMedico) {
            existe = 1;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return existe;
}

//Funcion que añade una cita, dandole los parametros y usando las 2 funciones anteriores para que verificar que el
//id de medico y paciente existen.
void agregarCita(sqlite3 *db) {
    int idPaciente, idMedico;
    char fecha[11];
    char estado[20];

    // Verificar paciente
    do {
        printf("Ingrese el ID del paciente: ");
        scanf("%d", &idPaciente);

        if (!verificarPacienteExiste(db, idPaciente)) {
            printf("Paciente no encontrado. Intentalo de nuevo.\n");
        } else {
            break;
        }
    } while (1);

    // Verificar médico
    do {
        printf("Ingrese el ID del medico: ");
        scanf("%d", &idMedico);

        if (!verificarMedicoExiste(db, idMedico)) {
            printf("Medico no encontrado. Intentalo de nuevo.\n");
        } else {
            break;
        }
    } while (1);

    // Leer fecha
    printf("Ingrese la fecha de la cita (YYYY-MM-DD): ");
    scanf("%s", fecha);

    // Leer estado
    printf("Ingrese el estado de la cita (ej: Programada): ");
    scanf("%s", estado);

    const char *sql = "INSERT INTO Cita (paciente_id, fecha, medico_id, estado) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la insercion: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, idPaciente);
    sqlite3_bind_text(stmt, 2, fecha, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idMedico);
    sqlite3_bind_text(stmt, 4, estado, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Cita agregada correctamente.\n");
    } else {
        printf("Error al agregar cita: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}



//Metodo que permite cambiar una cita (SOLO la fecha, el estado y el médico si quieres)
void modificarCita(sqlite3 *db) {
    int idCita;
    sqlite3_stmt *stmt;

    // Pedir ID y verificar que exista
    while (1) {
        printf("Ingrese el ID de la cita que desea modificar: ");
        scanf("%d", &idCita);

        const char *sqlCheck = "SELECT id, paciente_id, fecha, medico_id, estado FROM Cita WHERE id = ?";
        if (sqlite3_prepare_v2(db, sqlCheck, -1, &stmt, NULL) != SQLITE_OK) {
            printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
            return;
        }

        sqlite3_bind_int(stmt, 1, idCita);
        //en caso de q exista
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // se hace otro stmt para mostrar la cita y saber cómo está.
            Cita cita;
            cita.idCita = sqlite3_column_int(stmt, 0);
            cita.idPaciente = sqlite3_column_int(stmt, 1);
            strncpy(cita.fecha, (const char *)sqlite3_column_text(stmt, 2), sizeof(cita.fecha));
            cita.fecha[sizeof(cita.fecha) - 1] = '\0';
            cita.idMedico = sqlite3_column_int(stmt, 3);
            strncpy(cita.estado, (const char *)sqlite3_column_text(stmt, 4), sizeof(cita.estado));
            cita.estado[sizeof(cita.estado) - 1] = '\0';

            printf("\nCita actual:\n");
            imprimirCita(cita);
            sqlite3_finalize(stmt);
            break;
        } else {
            printf("No se encontró ninguna cita con ese ID. Intente nuevamente.\n");
            sqlite3_finalize(stmt);
        }
    }

    // Pedir nueva fecha
    char nuevaFecha[11];
    printf("Ingrese la nueva fecha (YYYY-MM-DD): ");
    scanf("%10s", nuevaFecha);

    // Preguntar si desea cambiar el médico
    int cambiarMedico = -1;
    int nuevoIdMedico = -1;
    char respuesta;

    //preguntamos si queremos cambiar el médico, pedimos el nuevo id de médico y verificamos si ese médico existe
    while (1) {
        printf("¿Desea cambiar el médico? (Y/N): ");
        scanf(" %c", &respuesta);
        if (respuesta == 'Y' || respuesta == 'y') {
            cambiarMedico = 1;

            // Verificar nuevo ID médico
            while (1) {
                printf("Ingrese el nuevo ID del médico: ");
                scanf("%d", &nuevoIdMedico);
                if (verificarMedicoExiste(db, nuevoIdMedico)) break;
                printf("El médico no existe. Intentalo de nuevo.\n");
            }
            break;
        } else if (respuesta == 'N' || respuesta == 'n') {
            cambiarMedico = 0;
            break;
        } else {
            printf("Respuesta no válida. Use 'Y' o 'N'.\n");
        }
    }

    // Pedir nuevo estado
    char nuevoEstado[20];
    printf("Ingrese el nuevo estado: ");
    scanf("%s", nuevoEstado);

    // Creo consulta dinámica (Se puede cambiar dependiendo de si queremos cambiar o no el médico)
    const char *sqlBase = "UPDATE Cita SET fecha = ?, estado = ?%s WHERE id = ?";
    char sqlFinal[256];
    //si se quiere cambiar el médico, le añadimos el código que queramos
    snprintf(sqlFinal, sizeof(sqlFinal),
             sqlBase, (cambiarMedico ? ", medico_id = ?" : ""));

    // Preparar sentencia final
    if (sqlite3_prepare_v2(db, sqlFinal, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la actualización: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Enlazar parámetros
    sqlite3_bind_text(stmt, 1, nuevaFecha, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, nuevoEstado, -1, SQLITE_TRANSIENT);

    //aquí, dependiendo de si queremos cambiar el id_medico o no, cambiaríamos el lugar en el que insertamos los datos
    //(ya que el stmt sería más grande.)
    int index = 3;
    if (cambiarMedico) {
        sqlite3_bind_int(stmt, index++, nuevoIdMedico);
    }

    sqlite3_bind_int(stmt, index, idCita);

    // Ejecutar la actualización
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Cita modificada con exito.\n");
    } else {
        printf("Error al modificar la cita: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void eliminarCita(sqlite3 *db) {
    int idCita;
    printf("Ingrese el ID de la cita que desea eliminar: ");
    scanf("%d", &idCita);
    while (getchar() != '\n'); 

    //Saber si la cita existe
    const char* sqlVerificar = "SELECT id FROM Cita WHERE id = ?;";
    sqlite3_stmt* stmtVerificar;

    if (sqlite3_prepare_v2(db, sqlVerificar, -1, &stmtVerificar, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta de verificación: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmtVerificar, 1, idCita);

    int resultVerificar = sqlite3_step(stmtVerificar);
    if (resultVerificar != SQLITE_ROW) {
        // Si no se encuentra la cita
        printf("No se encontró una cita con el ID %d.\n", idCita);
        sqlite3_finalize(stmtVerificar);
        return;
    }

    sqlite3_finalize(stmtVerificar);

    //Eliminar la cita si existe
    const char* sqlEliminar = "DELETE FROM Cita WHERE id = ?;";
    sqlite3_stmt* stmtEliminar;

    if (sqlite3_prepare_v2(db, sqlEliminar, -1, &stmtEliminar, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta de eliminación: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmtEliminar, 1, idCita);

    int resultEliminar = sqlite3_step(stmtEliminar);
    if (resultEliminar == SQLITE_DONE) {
        printf("Cita con ID %d eliminada correctamente.\n", idCita);
    } else {
        printf("Error al eliminar la cita: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmtEliminar);
}