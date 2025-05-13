#include "menus.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "funcionesMenu.h"
#include "../bd/sqlite3.h"

// Función para iniciar sesión
void menuInicioSesion() {
    int opcion;
    do {
        printf("\n----------- Bienvenido/a ------------\n");
        printf("1. INICIAR SESION\n");
        printf("2. SALIR\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("\n[Inicio de Sesion] Ingrese su ID y contrasenya...\n");
            escribirLog("Inicio de sesion."); //Se escribe el log en el fichero
            // TODO - Implementar autenticación
            char nom[10];
            //printf("Intruduzca su nombre: ");
            //sscanf("%d", &nom);
            //registrarInicioSesion(nom);
            //return 1;  // Retorna 1 si el usuario inicia sesión
            iniciarSesion();
            
        } else if (opcion == 2) {
            printf("\nSaliendo del sistema...\n");
            //return 0;  // Retorna 0 para salir
            

        } else {
            printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 2);
    //return 0;
}

void menuOpciones() {
    int opcion;
    do {
        printf("\n----------- MENÚ PRINCIPAL ------------\n");
        printf("1. Gestion de Pacientes\n");
        printf("2. Gestion de Citas\n");
        printf("3. Gestion de Historia Clinica\n");
        printf("4. Salir y Cerrar Sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuGestionPacientes();
                break;
            case 2:
                menuGestionCitas();
                break;
            case 3:
                menuGestionHistorial();
                break;
            case 4:
                printf("\n[Cierre de Sesion] Ha cerrado sesion correctamente.\n");
                escribirLog("Cierre de sesion."); //Se escribe el log en el fichero
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);
}

void menuGestionPacientes() {
    int opcion;

    sqlite3 *db;
    if (sqlite3_open("BD_HOSPITAL", &db) != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return;
    }

    do {
        printf("\n------- GESTION DE PACIENTES -------\n");
        printf("1. Agregar Paciente\n");
        printf("2. Buscar Paciente\n");
        printf("3. Modificar Paciente\n");
        printf("4. Eliminar Paciente\n");
        printf("5. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);


        switch (opcion) {
            case 1:
                printf("\n[Agregar Paciente] Ingrese los datos del paciente...\n");
                agregarPaciente(db);

                break;
            case 2:
                printf("\n[Buscar Paciente] Ingrese el ID del paciente...\n");
                buscarPaciente(db);
                break;
            case 3:
                printf("\n[Modificar Paciente] Ingrese el ID del paciente a modificar...\n");
                modificarPaciente(db);
                break;
            case 4:
                printf("\n[Eliminar Paciente] Ingrese el ID del paciente a eliminar...\n");
                eliminarPaciente(db);
                break;
            case 5:
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 5);

    sqlite3_close(db);
}

void menuGestionCitas() {
    int opcion;
    sqlite3 *db;
    if (sqlite3_open("BD_HOSPITAL", &db) != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return;
    }

    do {
        printf("\n-------- GESTION DE CITAS --------\n");
        printf("1. Consultar Citas de Paciente\n");
        printf("2. Consultar Citas por Medico\n");
        printf("3. Agregar Cita\n");
        printf("4. Modificar Cita\n");
        printf("5. Eliminar Cita\n");
        printf("6. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                consultarCitasPorPaciente(db);
                break;
            case 2:
                consultarCitasPorMedico(db);
                break;
            case 3:
                agregarCita(db);
                break;
            case 4:
                modificarCita(db);
                break;
            case 5:
               eliminarCita(db);
                break;
            case 6:
            printf("\nVolviendo al Menu Principal...\n");
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 6);

    printf("\nPresione Enter para continuar...\n");
    getchar(); // Espera a que el usuario presione Enter
    
    sqlite3_close(db);

}

void menuGestionHistorial() {
    int opcion;
    sqlite3 *db;
    if (sqlite3_open("BD_HOSPITAL", &db) != SQLITE_OK) {
        printf("No se pudo abrir la base de datos.\n");
        return;
    }

    do {
        printf("\n------ HISTORIAL CLINICO ------\n");
        printf("1. Consultar Historial\n");
        printf("2. Agregar Registro Medico\n");
        printf("3. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                consultarHistorialDelPaciente(db);
                break;
            case 2:
                modificarHistorial(db);
                break;
            case 3:
                eliminarHistorial(db);
                break;
            case 4:
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

