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
    string direccion;
    double latitud;
    double longitud;
    Paciente **listaPacientes;
    Profesional **listaProfesionales;
    Turno **listaTurno;
    Especialidad **listaEspecialidades;
    int cantidadPacientes, cantidadProfe, cantidadTurno, cantidadEspecialidad;
    int capacidadPacientes, capacidadProfe, capacidadTurno, capacidadEspecialidad;

public:
    Sanatorio(string nombre, string direccion, double lat =0.0, double lon = 0.0) :
    nombre(nombre), direccion(direccion), latitud (lat), longitud (lon),
    listaEspecialidades(nullptr), listaPacientes(nullptr), listaProfesionales(nullptr),
    listaTurno(nullptr), cantidadPacientes(0), cantidadProfe(0), cantidadTurno(0), cantidadEspecialidad(0),
    capacidadPacientes(0), capacidadProfe(0), capacidadTurno(0), capacidadEspecialidad(0){}
    const string& getNombre() const { return nombre; }
    const string& getDireccion() const { return direccion; }
    double getLatitud() const { return latitud; }
    double getLongitud() const { return longitud; }
    void setCoordenadas(double lat, double lon) { latitud = lat; longitud = lon; }

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

    bool tieneEspecialidad(int idEspecialidad) const;
    bool tieneProfesional(int idProfesional) const;
    void eliminarEspecialidad(int idEspecialidad);
    void eliminarProfesional(int idProfesional);

    ~Sanatorio() {}
};
#endif // SISTEMASANATORIO_SANATORIO_H
