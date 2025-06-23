#ifndef CITA_H_
#define CITA_H_

#include <string>

using namespace std;

class Cita {
    int id;
    int idPaciente;
    int idMedico;
    string fechaHora;
    string estado;

public:
    Cita(int id, int idPaciente, int idMedico, const string& fechaHora, const string& estado);
    Cita(): id(0), idPaciente(0), idMedico(0), fechaHora(""), estado("") {};
    ~Cita();

    int getId() const;
    int getIdPaciente() const;
    int getIdMedico() const;
    string getFechaHora() const;
    string getEstado() const;

    string toString() const;
    static Cita fromCSV(const string& datos, bool esModificar);
};

#endif
