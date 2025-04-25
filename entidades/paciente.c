#include <stdio.h>
#include "paciente.h"

void imprimirPaciente(Paciente p){
    printf("ID: %i, NOMBRE: %s, FECHA DE NACIMIENTO: %s, DIRECCION: %s, TFNO: %i",
        p.id,p.nombre,p.fecha_nac,p.direccion,p.tlfn);
}