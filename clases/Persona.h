#include <string>
#ifndef PERSONA_H_
#define PERSONA_H_
using namespace std;

class Persona {
protected:
    int id;
    string nombre;
public:
    Persona(int id, const std::string& nombre);

    virtual void imprimir() const = 0;
    virtual ~Persona();

    int getId() const;
    string getNombre() const;
};

#endif
