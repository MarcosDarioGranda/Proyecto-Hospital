#include <stdio.h>
#include "paciente.h"

void imprimirPaciente(Paciente p){
    printf("ID: %i\n NOMBRE: %s\n FECHA DE NACIMIENTO: %s\n DIRECCION: %s\n TFNO: %i\n",
        p.id,p.nombre,p.fecha_nac,p.direccion,p.tlfn);
}