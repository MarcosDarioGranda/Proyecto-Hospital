#include "HistorialMedico.h"
#include <sstream>

HistorialMedico::HistorialMedico(int id, int idPaciente, const string& antecedente) {

}

HistorialMedico::~HistorialMedico() {}

int HistorialMedico::getId() const { 
    return id; 
}
int HistorialMedico::getIdPaciente() const { 
    return idPaciente; 
}
string HistorialMedico::getAntecedente() const { 
    return antecedente; 
}

string HistorialMedico::toString() const {
    stringstream ss;
    ss << "Historial ID: " << id
       << " | Paciente ID: " << idPaciente
       << " | Descripción: " << antecedente;
    return ss.str();
}

HistorialMedico HistorialMedico::fromCSV(const string& datosCSV) {
    stringstream ss(datosCSV);
    string idStr, idPacStr, descripcion, fecha;

    getline(ss, idStr, ',');
    getline(ss, idPacStr, ',');
    getline(ss, descripcion, ',');
    getline(ss, fecha);

    return HistorialMedico(
        stoi(idStr),
        stoi(idPacStr),
        descripcion
    );
}