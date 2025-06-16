#include "Paciente.h"
#include <iostream>
using namespace std;
Paciente::Paciente(int id, const string& nombre,
            const string& fechaNacimiento,
            const string& direccion,
            int telefono): Persona(id, nombre) {
            this->fechaNacimiento = fechaNacimiento;
            this->direccion = direccion;
            this->telefono = telefono;
        }
Paciente::~Paciente(){

}

void Paciente::imprimir() const {
        std::cout << "[Paciente] ID: " << id
                  << ", Nombre: " << nombre
                  << ", Fecha de nacimiento: " << fechaNacimiento
                  << ", Dirección: " << direccion
                  << ", Teléfono: " << telefono << std::endl;
}