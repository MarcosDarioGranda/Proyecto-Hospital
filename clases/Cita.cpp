#include "Cita.h"
#include <sstream>
#include <vector>

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

Cita Cita::fromCSV(const std::string& datosCSV, bool esModificar) {
    std::stringstream ss(datosCSV);
    std::string campo;
    std::vector<std::string> campos;

    while (std::getline(ss, campo, ',')) {
        campos.push_back(campo);
    }

    if (!esModificar) {
        // Formato agregar: paciente_id,fecha,medico_id,estado (4 campos)
        if (campos.size() != 4) {
            throw std::invalid_argument("Formato incorrecto para agregar cita");
        }
        int paciente_id = std::stoi(campos[0]);
        std::string fecha = campos[1];
        int medico_id = std::stoi(campos[2]);
        std::string estado = campos[3];

        // id = -1 porque se asigna en BD
        return Cita(-1, paciente_id, medico_id, fecha, estado);

    } else {
        // Formato modificar: id,fecha,estado[,medico_id]
        if (campos.size() < 3 || campos.size() > 4) {
            throw std::invalid_argument("Formato incorrecto para modificar cita");
        }
        int id = std::stoi(campos[0]);
        std::string fecha = campos[1];
        std::string estado = campos[2];
        int medico_id = -1;  // por defecto sin cambio

        if (campos.size() == 4) {
            medico_id = std::stoi(campos[3]);
        }

        // Construir cita con id y campos opcionales
        // Puedes crear un constructor que admita campos opcionales
        return Cita(id, -1, medico_id, fecha, estado); // paciente_id no cambia al modificar
    }
}