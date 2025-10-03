#pragma once
#include <string>

class Persona
{
protected:
    int id;
    std::string nombre;
    std::string apellido;

public:
    Persona(int id, const std::string &nombre, const std::string &apellido)
        : id(id), nombre(nombre), apellido(apellido) {}
    virtual ~Persona() = default;

    int getId() const { return id; }
    const std::string &getNombre() const { return nombre; }
    const std::string &getApellido() const { return apellido; }
    virtual void mostrar() = 0;
};
