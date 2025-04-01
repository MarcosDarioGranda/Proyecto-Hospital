#ifndef MENUS_H_
#define MENUS_H_
#include "menus.h"

typedef struct {
    int id;
    char* nombre;
    int tNom;
    char* contrasenya;
    int tContra;
}Usuario;

void menuOpciones();
int menuInicioSesion();
#endif