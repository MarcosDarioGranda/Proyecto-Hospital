#include <string>
#ifndef FUNCIONES_PACIENTES_H
#define FUNCIONES_PACIENTES_H
#include "../lib/src/bd/sqlite3.h"


using namespace std;

#pragma once
static sqlite3* abrirBaseDeDatos();
string listarPacientes();
string buscarPacientePorID(const string& id_str);
string anyadirPaciente(const string& datos);
string modificarPaciente(const string& datos);
string eliminarPaciente(const string& id_str);
string procesarComandoPacientes(const string& entrada);
void menuPacientes();

#endif