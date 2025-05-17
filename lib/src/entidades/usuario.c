#include <stdio.h>
#include "usuario.h"


void imprimirUsuario(Usuario u){
    printf("ID: %i, NOMBRE: %s, CORREO: %s, ROL: %s, ESPECIALIDAD: %s \n", u.id,u.nombre,u.correo,u.rol,u.especialidad);
}