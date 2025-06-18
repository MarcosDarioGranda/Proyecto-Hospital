#ifndef USUARIO_H_
#define USUARIO_H_
#include "Persona.h"
using namespace std;

class Paciente : public Persona {
    string fechaNacimiento;
    string direccion;
    int telefono;
public:
    Paciente(int id, const string& nombre,
             const string& fechaNacimiento,
             const string& direccion,
             int telefono);
    ~Paciente();
    void imprimir() const override;
    string toString() const;

    static Paciente fromCSV(const string& datos);

    // Getters si no los tenés:
    string getFechaNacimiento() const { return fechaNacimiento; }
    string getDireccion() const { return direccion; }
    int getTelefono() const { return telefono; }
};

#endif
