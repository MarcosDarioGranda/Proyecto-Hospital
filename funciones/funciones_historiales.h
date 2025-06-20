#ifndef FUNCIONES_HISTORIALES_H_
#define FUNCIONES_HISTORIALES_H_
#include "../lib/src/bd/sqlite3.h"
#include <string>
using namespace std;

#pragma once
sqlite3* abrirBaseDeDatos();
string consultarHistorialDelPaciente(const string& id_str);
string agregarHistorial(const string& datos);
string modificarHistorial(const string& datos);
string eliminarHistorial(const string& id_str);

#endif
