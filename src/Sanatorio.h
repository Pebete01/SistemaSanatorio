//
// Created by tomyl on 10/9/2025.
//
#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"

using namespace std;

#ifndef SISTEMASANATORIO_SANATORIO_H
#define SISTEMASANATORIO_SANATORIO_H

class Turno;
class Sanatorio
{
private:
    string nombre;
    string ubicacion;//VER SI SACAR
    double latitud;
    double longitud;
    Paciente **listaPacientes;
    Profesional **listaProfesionales;
    Turno **listaTurno;
    Especialidad **listaEspecialidades;
    int cantidadPacientes, cantidadProfe, cantidadTurno, cantidadEspecialidad;
    int capacidadPacientes, capacidadProfe, capacidadTurno, capacidadEspecialidad;

public:
    Sanatorio(string nombre, string ubicacion, double lat, double lon) : nombre(nombre), ubicacion(ubicacion), latitud (lat), longitud (lon), listaEspecialidades(nullptr), listaPacientes(nullptr), listaProfesionales(nullptr), listaTurno(nullptr) {}

    double getLatitud() const { return latitud; }
    double getLongitud() const { return longitud; }
    string getNombre() const { return nombre;}

    void agrandarListaPaciente();
    void agregarPaciente(Paciente *p);
    void agrandarListaProfesionales();
    void agregarProfesional(Profesional *p);
    void agrandarListaTurnos();
    void agregarTurnos(Turno *p);
    void agrandarListaEspecialidad();
    void agregarEspecialidad(Especialidad *p);

    void sacarProfesional();
    void sacarEspecialidad();
    void cancelarTurno();
    void mostrarSanatorio();

    ~Sanatorio() {}
};
#endif // SISTEMASANATORIO_SANATORIO_H
