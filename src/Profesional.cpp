#include "iostream"
#include "string"
#include "Profesional.h"
using namespace std;

void Profesional::modificarEstado()
{
    if (!estado)
    {
        estado = true;
    }
    else
    {
        estado = false;
    }
}

void Profesional::mostrar()
{
    cout << "ID: " << id << " Dr/Dra: " << nombre << " " << apellido << " numero de profesional: " << numeroDeProfesional << endl;
    especialidad.mostrar();
}

void Profesional::instanciasVivas()
{
    cout << "Profesionales totales: " << numProfesionales << endl;
}

Especialidad Profesional::getEspecialidad()
{
    return especialidad;
}

int Profesional::getNumueroProfesional() {
    return numeroDeProfesional;
}