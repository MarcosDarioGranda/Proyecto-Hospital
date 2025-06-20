#ifndef HISTORIALMEDICO_H_
#define HISTORIALMEDICO_H_

#include <string>
using namespace std;

class HistorialMedico {
    int id;
    int idPaciente;
    string antecedente;

public:
    HistorialMedico(int id, int idPaciente, const string& antecedente);
    ~HistorialMedico();

    int getId() const;
    int getIdPaciente() const;
    string getAntecedente() const;

    string toString() const;
    static HistorialMedico fromCSV(const string& datos);
};

#endif