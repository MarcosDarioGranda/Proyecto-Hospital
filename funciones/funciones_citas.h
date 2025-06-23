#ifndef FUNCIONES_CITAS_H_
#define FUNCIONES_CITAS_H_
#include "../lib/src/bd/sqlite3.h"
#include <string>
using namespace std;

#pragma once
static sqlite3* abrirBaseDeDatos();
string consultarCitasPorPaciente(const string& id_str);
string consultarCitasPorMedico(const string& id_str);
bool verificarPacienteExiste(sqlite3 *db, int idPaciente);
bool verificarMedicoExiste(sqlite3 *db, int idMedico);
string agregarCita(const string& datos);
string modificarCita(const string& datos);
string eliminarCita(const string& id_str);
string procesarComandoCitas(const string& entrada);

#endif