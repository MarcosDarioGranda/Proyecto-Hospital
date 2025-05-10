#ifndef FUNCIONESMENU_H_
#define FUNCIONESMENU_H_
#include  "../bd/sqlite3.h"
typedef struct {
    int dni;
    char nombre[50];
    char contrasenya[50];
    char email[50];
    char rol[50];
    char especialidad[50];
} UsuarioInicio;

void iniciarSesion();
void ejecutarSQL(sqlite3* db, const char* sql);
//Funciones Gestion de Pacientes
void agregarPaciente(sqlite3 *db);
void buscarPaciente(sqlite3 *db);
void modificarPaciente(sqlite3 *db);
void eliminarPaciente(sqlite3 *db);
//Funciones Gestion de Citas
void consultarCitasPorPaciente(sqlite3 *db);
void consultarCitasPorMedico(sqlite3 *db);
int verificarPacienteExiste(sqlite3 *db, int idPaciente);
int verificarMedicoExiste(sqlite3 *db, int idMedico);
void agregarCita(sqlite3 *db);
void modificarCita(sqlite3 *db);
void eliminarCita(sqlite3 *db);
#endif