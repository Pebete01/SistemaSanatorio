#pragma once
#include <string>

class Persona
{
protected:
    int id;
    std::string nombre;
    std::string apellido;
    std::string mail; // <-- NUEVO CAMPO

public:
    Persona(int id, const std::string &nombre, const std::string &apellido, const std::string& mail)
            : id(id), nombre(nombre), apellido(apellido), mail(mail) {}
    virtual ~Persona() = default;

    int getId() const { return id; }
    const std::string &getNombre() const { return nombre; }
    const std::string &getApellido() const { return apellido; }
    const std::string &getMail() const { return mail; } // <-- NUEVO GETTER
    virtual void mostrar() = 0;
};
