#ifndef HOSPITAL_H
#define HOSPITAL_H

#ifdef __cplusplus
extern "C" {
#endif

// Devuelve un buffer con malloc() que contiene todos los historiales
// del paciente (idPaciente). El llamante debe free() el resultado.
// En caso de error, devuelve una cadena con mensaje de error.
char* hospital_consultar_historial(int idPaciente);

// Inserta un nuevo antecedente para idPaciente.
// Devuelve 0 si tuvo éxito, o un negativo en caso de fallo.
int hospital_agregar_historial(int idPaciente, const char *antecedente);

#ifdef __cplusplus
}
#endif

#endif // HOSPITAL_H