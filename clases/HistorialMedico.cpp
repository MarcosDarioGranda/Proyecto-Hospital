#include "HistorialMedico.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <vector>

// Función auxiliar para eliminar espacios al inicio y fin de un string
static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

HistorialMedico::HistorialMedico(int id, int idPaciente, const std::string& antecedente) {
    this->id = id;
    this->idPaciente = idPaciente;
    this->antecedente = antecedente;
}

HistorialMedico::~HistorialMedico() {}

int HistorialMedico::getId() const { 
    return id; 
}

int HistorialMedico::getIdPaciente() const { 
    return idPaciente; 
}

std::string HistorialMedico::getAntecedente() const { 
    return antecedente; 
}

std::string HistorialMedico::toString() const {
    std::stringstream ss;
    ss << "Historial ID: " << id
       << " | Paciente ID: " << idPaciente
       << " | Descripción: " << antecedente;
    return ss.str();
}

HistorialMedico HistorialMedico::fromCSV(const std::string& datosCSV) {
    std::stringstream ss(datosCSV);
    std::string campo;
    std::vector<std::string> campos;

    while (std::getline(ss, campo, ',')) {
        campos.push_back(trim(campo));
    }

    if (campos.size() == 2) {
        int idPaciente;
        try {
            idPaciente = std::stoi(campos[0]);
        } catch (...) {
            throw std::invalid_argument("Error: El paciente_id no es un número válido.");
        }
        const std::string& antecedente = campos[1];
        if (antecedente.empty()) {
            throw std::invalid_argument("Error: El campo antecedente no puede estar vacío.");
        }
        return HistorialMedico(0, idPaciente, antecedente);

    } else if (campos.size() == 3) {
        int id, idPaciente;
        try {
            id = std::stoi(campos[0]);
        } catch (...) {
            throw std::invalid_argument("Error: El id no es un número válido.");
        }
        try {
            idPaciente = std::stoi(campos[1]);
        } catch (...) {
            throw std::invalid_argument("Error: El paciente_id no es un número válido.");
        }
        const std::string& antecedente = campos[2];
        if (antecedente.empty()) {
            throw std::invalid_argument("Error: El campo antecedente no puede estar vacío.");
        }
        return HistorialMedico(id, idPaciente, antecedente);

    } else {
        throw std::invalid_argument("Error: Número de campos inválido. Se esperaban 2 o 3 campos.");
    }
}
