//
// Created by tomyl on 10/9/2025.
//
#include "iostream"
#include "string"
#include "Persona.h"
#include "Especialidad.h"
using namespace std;
#ifndef SISTEMASANATORIO_PACIENTE_H
#define SISTEMASANATORIO_PACIENTE_H

class Paciente : public Persona
{
private:
    string obraSocial;
    int numeroDeAfiliado;
    bool estadoPaciente;
    static int cantidad;

public:
    Paciente(int id, string nombre, string apellido, int numeroAfiliado, string obraSocial)
        : Persona(id, nombre, apellido), numeroDeAfiliado(numeroAfiliado), obraSocial(obraSocial), estadoPaciente(true)
    {
        cantidad++;
    }
    void modificarEstado();
    void mostrar();
    void instanciasVivas();

    ~Paciente()
    {
        cantidad--;
    }
};
#endif // SISTEMASANATORIO_PACIENTE_H
