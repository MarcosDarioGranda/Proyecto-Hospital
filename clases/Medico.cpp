#include "Medico.h"
#include <iostream>

Medico::Medico(int id, const string& nombre,
            const string& correo,
            const string& contrasenya,
            const string& rol,
            const string& especialidad) :Persona(id, nombre) {
                this->correo = correo;
                this->contrasenya = contrasenya;
                this->rol = rol;
                this->especialidad = especialidad;
        }

Medico::~Medico() {

}

void Medico::imprimir() const  {
        std::cout << "[Usuario] ID: " << id
                  << ", Nombre: " << nombre
                  << ", Correo: " << correo
                  << ", Rol: " << rol
                  << ", Especialidad: " << especialidad << std::endl;
    }