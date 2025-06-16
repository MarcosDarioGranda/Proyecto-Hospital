#include "Persona.h"
    Persona::Persona(int id, const string& nombre) {
        this->id = id;
        this->nombre = nombre;
    }
Persona::~Persona() {

}

int Persona::getId() const { 
    return id; 
}

string Persona::getNombre() const { 
    return nombre;
}