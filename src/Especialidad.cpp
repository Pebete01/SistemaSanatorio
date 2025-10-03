#include "Especialidad.h"
#include <iostream>

Especialidad::Especialidad(int id_, const std::string &nombre_)
    : id(id_), nombre(nombre_) {}

int Especialidad::getId() const noexcept { return id; }
const std::string &Especialidad::getNombre() const noexcept { return nombre; }
bool Especialidad::getEstado() const noexcept { return estado; }

void Especialidad::toggleEstado() noexcept { estado = !estado; }

void Especialidad::mostrar() const
{
    std::cout << "ID: " << id
              << " | Especialidad: " << nombre
              << " | Estado: " << (estado ? "Activa" : "Inactiva")
              << '\n';
}
