#ifndef USUARIO_H_
#define USUARIO_H_
typedef struct
{
    int id;
    char* nombre[40];
    char* correo[40];
    char* contrasenya[40];
    char* rol[20];
    char* especialidad[20];

}Usuario;

void imprimirUsuario(Usuario u);
#endif