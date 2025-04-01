#include "menus.h"
#include <stdio.h>
#include <stdlib.h>
#include "funcionesMenu.h"

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
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 4);
}

void menuGestionPacientes() {
    int opcion;
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
                break;
            case 2:
                printf("\n[Buscar Paciente] Ingrese el ID del paciente...\n");
                break;
            case 3:
                printf("\n[Modificar Paciente] Ingrese el ID del paciente a modificar...\n");
                break;
            case 4:
                printf("\n[Eliminar Paciente] Ingrese el ID del paciente a eliminar...\n");
                break;
            case 5:
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void menuGestionCitas() {
    int opcion;
    do {
        printf("\n-------- GESTION DE CITAS --------\n");
        printf("1. Consultar Citas\n");
        printf("2. Agregar Cita\n");
        printf("3. Modificar Cita\n");
        printf("4. Eliminar Cita\n");
        printf("5. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n[Consultar Citas] Mostrando citas programadas...\n");
                break;
            case 2:
                printf("\n[Agregar Cita] Ingrese los datos de la nueva cita...\n");
                break;
            case 3:
                printf("\n[Modificar Cita] Ingrese el ID de la cita a modificar...\n");
                break;
            case 4:
                printf("\n[Eliminar Cita] Ingrese el ID de la cita a eliminar...\n");
                break;
            case 5:
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 5);
}

void menuGestionHistorial() {
    int opcion;
    do {
        printf("\n------ HISTORIAL CLINICO ------\n");
        printf("1. Consultar Historial\n");
        printf("2. Agregar Registro Medico\n");
        printf("3. Volver al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n[Consultar Historial] Ingrese el ID del paciente...\n");
                break;
            case 2:
                printf("\n[Agregar Registro Médico] Ingrese los datos...\n");
                break;
            case 3:
                return;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 3);
}

