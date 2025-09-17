#ifndef SISTEMASANATORIO_TURNO_H
#define SISTEMASANATORIO_TURNO_H

#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
using namespace std;

class Turno
{
private:
    int id;
    string fecha, hora, diagnosticoAsistido;
    Paciente cliente;
    Profesional medico;

public:
    Turno(int id, string fecha, string hora, string diagnostico, Paciente cliente, Profesional medico)
        : id(id), fecha(fecha), hora(hora), diagnosticoAsistido(diagnostico), cliente(cliente), medico(medico) {}

    ~Turno() {}

    void cancelar();
    void reprogramar(string nuevaFecha, string nuevaHora);
    void mostrar();
};

#endif // SISTEMASANATORIO_TURNO_H