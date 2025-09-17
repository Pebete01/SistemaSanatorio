#include "iostream"
#include "string"
#include "Persona.h"
#include "Especialidad.h"
using namespace std;

class Profesional : public Persona
{
private:
    bool estado;
    int numeroDeProfesional;
    Especialidad especialidad;
    static int numProfesionales;

public:
    Profesional(int numero, Especialidad especialidad, int id, string nombre, string apellido) : Persona(id, nombre, apellido), estado(true), numeroDeProfesional(numero), especialidad(especialidad)
    {
        numProfesionales++;
    }
    void modificarEstado();

    void mostrar();

    Especialidad getEspecialidad();
    void static instanciasVivas();

    ~Profesional()
    {
        numProfesionales--;
    }
};