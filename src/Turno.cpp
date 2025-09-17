#include "Turno.h"
#include "iostream"
#include "string"
using namespace std;

void Turno::cancelar()
{
    cout << "El turno con ID: " << id << " ha sido cancelado." << endl;
}

void Turno::reprogramar(string nuevaFecha, string nuevaHora)
{
    fecha = nuevaFecha;
    hora = nuevaHora;
    cout << "El turno con ID: " << id << " ha sido reprogramado a " << fecha << " a las " << hora << "." << endl;
}

void Turno::mostrar()
{
    cout << "Turno ID: " << id << endl;
    cout << "El dia: " << fecha << " a las " << hora << endl;

    cout << "Paciente: ";
    cliente.mostrar();
    
    cout << "Profesional: ";
    medico.mostrar();
}