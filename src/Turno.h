#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
using namespace std;

class Turno{
private:
    int idDeTurno;
    string fechas;
    string hora;
    string diagnosticoAsistido;
    Paciente cliente;
    Profesional medico;
public:
    Turno(int id, string fecha, string hora, string diagnostico, Paciente cliente, Profesional medico) : idDeTurno(id), fechas(fecha), hora(hora), diagnosticoAsistido(diagnostico),cliente(cliente),medico(medico) {}

    ~Turno(){}
};