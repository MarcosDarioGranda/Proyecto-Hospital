#include "Paciente.h"
#include <iostream>
#include <sstream>

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
        cout << "[Paciente] ID: " << id
                  << ", Nombre: " << nombre
                  << ", Fecha de nacimiento: " << fechaNacimiento
                  << ", Direccion: " << direccion
                  << ", Telefono: " << telefono << endl;
}

string Paciente::toString() const {
        return "[Paciente] ID: " + to_string(id) +
               ", Nombre: " + nombre +
               ", Fecha de nacimiento: " + fechaNacimiento +
               ", Direccion: " + direccion +
               ", Telefono: " + to_string(telefono);
}

Paciente Paciente::fromCSV(const string& datos) {
    stringstream ss(datos);
    string nombre, fecha, direccion, tlfn;

    getline(ss, nombre, ',');
    getline(ss, fecha, ',');
    getline(ss, direccion, ',');
    getline(ss, tlfn);

    // id = 0 porque la base de datos lo asigna automáticamente
    return Paciente(0, nombre, fecha, direccion, stoi(tlfn));
}