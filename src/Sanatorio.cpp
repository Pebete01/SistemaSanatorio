//
// Created by tomyl on 10/9/2025.
//
#include "iostream"
#include "string"
#include "Sanatorio.h"
using namespace std;

void Sanatorio::agrandarListaPaciente() {
    int capacidadNuevaPacientes = (capacidadPacientes ==0) ? 4: capacidadPacientes*2;
    Paciente **listaNueva = new Paciente * [capacidadNuevaPacientes];

    for (int i = 0; i < cantidadPacientes; ++i) {
        listaNueva[i] = listaPacientes[i];
    }
    delete[] listaPacientes;
    listaPacientes = listaNueva;
    capacidadPacientes = capacidadNuevaPacientes;
}

void Sanatorio::agregarPaciente(Paciente *p) {
    if(capacidadPacientes == cantidadPacientes) agrandarListaPaciente();
    listaPacientes[cantidadPacientes++] = p;
}

