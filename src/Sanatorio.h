//
// Created by tomyl on 10/9/2025.
//
#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"
#include "Turno.h"
using namespace std;

#ifndef SISTEMASANATORIO_SANATORIO_H
#define SISTEMASANATORIO_SANATORIO_H
class Sanatorio
{
private:
    string nombre;
    string ubicacion;
    Paciente **listaPacientes;
    Profesional **listaProfesionales;
    Turno **listaTurno;
    Especialidad **listaEspecialidades;
    int cantidadPacientes, cantidadProfe, cantidadTurno, cantidadEspecialidad;
    int capacidadPacientes,capacidadProfe, capacidadTurno, capacidadEspecialidad;

public:
    Sanatorio(string nombre, string ubicacion) : nombre(nombre), ubicacion(ubicacion), listaEspecialidades(NULL), listaPacientes(NULL), listaProfesionales(NULL), listaTurno(NULL) {}

    void agrandarListaPaciente();
    void agregarPaciente(Paciente *p);
    void agrandarListaProfesionales();
    void agregarProfesional(Profesional * p);
    void agrandarListaTurnos();
    void agregarTurnos(Turno *p);
    void agrandarListaEspecialidad();
    void agregarEspecialidad(Especialidad *p);

    void sacarProfesional();
    ~Sanatorio() {}
};
#endif // SISTEMASANATORIO_SANATORIO_H
