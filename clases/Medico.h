#ifndef MEDICO_H_
#define MEDICO_H_
#include "Persona.h"
using namespace std;

class Medico : public Persona {
    string correo, contrasenya, rol, especialidad;
public:
    Medico(int id, const string& nombre,
            const string& correo,
            const string& contrasenya,
            const string& rol,
            const string& especialidad);
    ~Medico();
    void imprimir() const override;
};

#endif