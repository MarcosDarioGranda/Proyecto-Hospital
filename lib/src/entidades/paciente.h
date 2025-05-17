#ifndef PACIENTE_H_
#define PACIENTE_H_

typedef struct{
    int id;
    char *nombre[21];
    char *fecha_nac[11];
    char * direccion[31];
    int tlfn;
}Paciente;

void imprimirPaciente(Paciente p);

#endif