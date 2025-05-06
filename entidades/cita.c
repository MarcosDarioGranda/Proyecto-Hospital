#include <stdio.h>
#include "cita.h"


void imprimirCita(Cita c){
    printf("ID Cita: %i, FECHA DE CITA: %s, ID MÉDICO: %i, ESTADO: %s\n",
        c.idCita,c.fecha,c.idMedico,c.estado);
};
void imprimirCitaMedico(Cita c){
    printf("ID Cita: %i, FECHA DE CITA: %s, ID PACIENTE: %i, ESTADO: %s\n",
        c.idCita,c.fecha,c.idPaciente,c.estado);
}