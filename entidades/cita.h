#ifndef CITA_H_
#define CITA_H

typedef struct{
    int idCita;
    int idPaciente;
    char* fecha[11];
    int idMedico;
    char* estado[20];
}Cita;

void imprimirCita(Cita c);
void imprimirCitaMedico(Cita c);

#endif