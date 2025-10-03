#include "Paciente.h"
#include <ostream>
#include <iostream>

int Paciente::cantidad = 0;

void Paciente::mostrar() { std::cout << *this << '\n'; }

std::ostream &operator<<(std::ostream &os, const Paciente &p)
{
    os << "[" << p.getId() << "] " << p.getApellido() << ", " << p.getNombre()
       << " | OS: " << p.getObraSocial()
       << " | Afiliado: " << p.getNumeroDeAfiliado();
    return os;
}
bool Paciente::operator<(const Paciente &o) const
{
    if (getApellido() != o.getApellido())
        return getApellido() < o.getApellido();
    if (getNombre() != o.getNombre())
        return getNombre() < o.getNombre();
    return getId() < o.getId();
}
