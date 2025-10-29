#pragma once
#include <string>

class Persona
{
protected:
    int id;
    std::string nombre;
    std::string apellido;
    std::string mail;

public:
    Persona(int id, const std::string &nombre, const std::string &apellido, const std::string& mail)
            : id(id), nombre(nombre), apellido(apellido), mail(mail) {}
    virtual ~Persona() = default;

    int getId() const { return id; }
    const std::string &getNombre() const { return nombre; }
    const std::string &getApellido() const { return apellido; }
    const std::string &getMail() const { return mail; }
    virtual void mostrar() = 0;
};
