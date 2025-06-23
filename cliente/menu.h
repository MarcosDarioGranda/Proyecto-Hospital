#ifndef MENU_H
#define MENU_H

using namespace std;

void enviarComando(int socket, const string& comando);
string recibirRespuesta(int socket);
void mostrarMenuPacientes(int socket);
void mostrarMenuCitas(int socket);
void mostrarMenuHistorial(int socket);

#endif
