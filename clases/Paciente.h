#ifndef USUARIO_H_
#define USUARIO_H_
#include "Persona.h"
using namespace std;

class Paciente : public Persona {
    string fechaNacimiento;
    string direccion;
    int telefono;
public:
    Paciente(int id, const std::string& nombre,
             const std::string& fechaNacimiento,
             const std::string& direccion,
             int telefono);
    ~Paciente();
    void imprimir() const override;
    string toString() const;
};

#endif
