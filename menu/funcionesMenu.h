#ifndef MENUS_H_
#define MENUS_H_
#include "funcionesMenu.h"
#include  "../bd/sqlite3.h"
typedef struct {
    int dni;
    char nombre[50];
    char contrasenya[50];
    char email[50];
    char rol[50];
    char especialidad[50];
} UsuarioInicio;

void inicio();
void ejecutarSQL(sqlite3* db, const char* sql);
void registrarInicioSesion(const char *usuario);

#endif