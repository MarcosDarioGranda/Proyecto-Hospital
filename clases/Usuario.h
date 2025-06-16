#ifndef USUARIO_H_
#define USUARIO_H_
#include "Persona.h"
using namespace std;

class Usuario : public Persona {
    string correo, contrasenya, rol, especialidad;
public:
    Usuario(int id, const string& nombre,
            const string& correo,
            const string& contrasenya,
            const string& rol,
            const string& especialidad);
    ~Usuario();
    void imprimir() const override;
};

#endif