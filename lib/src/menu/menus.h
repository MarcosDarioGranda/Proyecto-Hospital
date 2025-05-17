#ifndef MENUS_H_
#define MENUS_H_

typedef struct {
    int id;
    char* nombre;
    int tNom;
    char* contrasenya;
    int tContra;
}Usuario;


void menuInicioSesion();
void menuOpciones();
void menuGestionPacientes();
void menuGestionCitas();
void menuGestionHistorial();

#endif