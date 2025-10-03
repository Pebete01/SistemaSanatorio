#include "Profesional.h"
#include <iostream>

int Profesional::numProfesionales = 0;

Profesional::Profesional(int numero,
                         const Especialidad &esp,
                         int id,
                         const std::string &nombre,
                         const std::string &apellido)
    : Persona(id, nombre, apellido),
      estado(true),
      numeroDeProfesional(numero),
      especialidad(esp)
{
    ++numProfesionales;
}

Profesional::~Profesional()
{
    --numProfesionales;
}

void Profesional::modificarEstado()
{
    estado = !estado;
}

void Profesional::mostrar()
{
    std::cout << "ID: " << getId()
              << " Dr/Dra: " << getNombre() << ' ' << getApellido()
              << " Numero de profesional: " << numeroDeProfesional
              << " Estado: " << (estado ? "Activo" : "Inactivo")
              << '\n';
}

int Profesional::getNumeroProfesional() const
{
    return numeroDeProfesional;
}

const Especialidad &Profesional::getEspecialidad() const
{
    return especialidad;
}

void Profesional::instanciasVivas()
{
    std::cout << "Profesionales vivos: " << numProfesionales << '\n';
}
