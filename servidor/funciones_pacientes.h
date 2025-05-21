#include <string>
#ifndef FUNCIONES_PACIENTES_H
#define FUNCIONES_PACIENTES_H

#pragma once
void menuPacientes();
void mostrarMenuPacientes(int socketCliente);
std::string procesarComando(const std::string &req);
#endif