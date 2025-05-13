#ifndef HISTORIAL_H
#define HISTORIAL_H

// Estructura para historial
typedef struct {
    int idHistorial;
    int idPaciente;
    char antecedente[256];
} Historial;

void imprimirHistorial(Historial h);
#endif