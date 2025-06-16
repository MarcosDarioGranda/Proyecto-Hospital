#include "Usuario.h"
#include <iostream>

Usuario::Usuario(int id, const string& nombre,
            const string& correo,
            const string& contrasenya,
            const string& rol,
            const string& especialidad) :Persona(id, nombre) {
                this->correo = correo;
                this->contrasenya = contrasenya;
                this->rol = rol;
                this->especialidad = especialidad;
        }

Usuario::~Usuario() {

}

void Usuario::imprimir() const  {
        std::cout << "[Usuario] ID: " << id
                  << ", Nombre: " << nombre
                  << ", Correo: " << correo
                  << ", Rol: " << rol
                  << ", Especialidad: " << especialidad << std::endl;
    }