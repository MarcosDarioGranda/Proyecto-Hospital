#include <stdio.h>
#include "historial.h"

void imprimirHistorial(Historial h) {
    printf("ID Historial: %i, ID PACIENTE: %i, ANTECEDENTE: %s\n",
        h.idHistorial, h.idPaciente, h.antecedente);
}

