#include "Cita.h"
#include <sstream>

Cita::Cita(int id, int idPaciente, int idMedico, const string& fechaHora, const string& estado) {
    this->id = id;
    this->idPaciente = idPaciente;
    this->idMedico = idMedico;
    this->fechaHora = fechaHora;
    this->estado = estado;
}

Cita::~Cita() {}

int Cita::getId() const { 
    return id; 
}
int Cita::getIdPaciente() const { 
    return idPaciente; 
}
int Cita::getIdMedico() const { 
    return idMedico; 
}
string Cita::getFechaHora() const { 
    return fechaHora; 
}
string Cita::getEstado() const { 
    return estado; 
}

string Cita::toString() const {
    stringstream ss;
    ss << "Cita ID: " << id
       << " | Paciente ID: " << idPaciente
       << " | Médico ID: " << idMedico
       << " | Fecha/Hora: " << fechaHora
       << " | Motivo: " << estado;
    return ss.str();
}

Cita Cita::fromCSV(const string& datosCSV) {
    stringstream ss(datosCSV);
    string idStr, idPacStr, idMedStr, fechaHora, motivo;

    getline(ss, idStr, ',');
    getline(ss, idPacStr, ',');
    getline(ss, idMedStr, ',');
    getline(ss, fechaHora, ',');
    getline(ss, motivo);

    return Cita(
        stoi(idStr),
        stoi(idPacStr),
        stoi(idMedStr),
        fechaHora,
        motivo
    );
}